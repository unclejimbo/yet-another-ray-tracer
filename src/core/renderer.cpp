#include <yart/core/renderer.h>
#include <embree3/rtcore.h>
#include <omp.h>
#include <iostream>
#include <numeric>

namespace yart
{

Sampler Renderer::_sampler;

int Renderer::render_tiled(const Scene& scene, RenderData& data)
{
    _num_rays.resize(omp_get_max_threads(), 0);
    const int num_tiles_x = (data.width + TILE_SIZE - 1) / TILE_SIZE;
    const int num_tiles_y = (data.height + TILE_SIZE - 1) / TILE_SIZE;
    const int num_tasks = num_tiles_x * num_tiles_y;
    int cnt = 0;
#pragma omp parallel for schedule(dynamic)
    for (int i = 0; i < num_tasks; ++i) {
        int thread_id = omp_get_thread_num();
        _render_task_tiled(scene, data, i, thread_id, num_tiles_x);
#pragma omp critical
        {
#pragma omp atomic
            ++cnt;
            std::cout << "Render progress: " << cnt << "/" << num_tasks
                      << std::endl;
        }
    }
    return std::accumulate(_num_rays.begin(), _num_rays.end(), 0);
}

void Renderer::_render_task_tiled(const Scene& scene,
                                  RenderData& data,
                                  int task_id,
                                  int thread_id,
                                  int num_tiles_x)
{
    const int tile_y = task_id / num_tiles_x;
    const int tile_x = task_id - tile_y * num_tiles_x;
    const int x0 = tile_x * TILE_SIZE;
    const int x1 = std::min(x0 + TILE_SIZE, data.width);
    const int y0 = tile_y * TILE_SIZE;
    const int y1 = std::min(y0 + TILE_SIZE, data.height);

    for (int y = y0; y < y1; ++y) {
        for (int x = x0; x < x1; ++x) {
            Eigen::Array3f color =
                _render_pixel(scene, data, x, y, _num_rays[thread_id]);
            auto r = static_cast<unsigned char>(
                std::clamp(color(0), 0.0f, 1.0f) * 255);
            auto g = static_cast<unsigned char>(
                std::clamp(color(1), 0.0f, 1.0f) * 255);
            auto b = static_cast<unsigned char>(
                std::clamp(color(2), 0.0f, 1.0f) * 255);
            if (data.interleaved) {
                data.pixels[3 * ((data.height - y - 1) * data.width + x) + 0] =
                    r;
                data.pixels[3 * ((data.height - y - 1) * data.width + x) + 1] =
                    g;
                data.pixels[3 * ((data.height - y - 1) * data.width + x) + 2] =
                    b;
            }
            else {
                data.pixels[(data.height - y - 1) * data.width + x] = r;
                data.pixels[data.width * data.height +
                            (data.height - y - 1) * data.width + x] = g;
                data.pixels[3 * data.width * data.height +
                            (data.height - y - 1) * data.width + x] = b;
            }
        }
    }
}

} // namespace yart
