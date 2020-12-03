#pragma once

#include <yart/core/common.h>
#include <yart/core/sampler.h>
#include <Eigen/Core>
#include <vector>

namespace yart
{

class Scene;

struct RenderData
{
    unsigned char* pixels;
    bool interleaved = false;
    int width = 0;
    int height = 0;
};

class YART_API Renderer
{
    static constexpr const int TILE_SIZE = 8;

public:
    virtual ~Renderer(){};
    int render_tiled(const Scene& scene, RenderData& data);

protected:
    virtual Eigen::Array3f _render_pixel(const Scene& scene,
                                         RenderData& data,
                                         int x,
                                         int y,
                                         int& num_rays) = 0;

protected:
    static Sampler _sampler;

private:
    void _render_task_tiled(const Scene& scene,
                            RenderData& data,
                            int task_id,
                            int thread_id,
                            int num_tiles_x);

private:
    std::vector<int> _num_rays;
};

} // namespace yart
