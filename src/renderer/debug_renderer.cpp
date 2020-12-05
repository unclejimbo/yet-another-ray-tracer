#include <yart/renderer/debug_renderer.h>
#include <yart/core/camera.h>
#include <yart/core/scene.h>
#include <yart/geometry/instance.h>
#include "../util/rayhit.h"

namespace yart
{

DebugRenderer::DebugRenderer(RenderMode mode) : _mode(mode)
{
    // TODO: better color palette handling
    _color_palette = {
        Eigen::Array3f(249, 65, 68),   Eigen::Array3f(243, 114, 44),
        Eigen::Array3f(248, 150, 30),  Eigen::Array3f(249, 199, 79),
        Eigen::Array3f(144, 190, 109), Eigen::Array3f(67, 170, 139),
        Eigen::Array3f(87, 117, 144)
    };
    for (auto& c : _color_palette) {
        c /= 255.0f;
    }
}

void DebugRenderer::generate_color_palette(int num)
{
    if (num < _color_palette.size()) { _color_palette.resize(num); }
    else if (num > _color_palette.size()) {
        size_t i = _color_palette.size();
        size_t m = i;
        _color_palette.resize(num);
        for (; i < _color_palette.size(); ++i) {
            _color_palette[i] = _color_palette[i % m];
            _color_palette[i](0) +=
                Renderer::_sampler.uniform_1d() * 0.6f - 0.3f;
            _color_palette[i](1) +=
                Renderer::_sampler.uniform_1d() * 0.6f - 0.3f;
            _color_palette[i](2) +=
                Renderer::_sampler.uniform_1d() * 0.6f - 0.3f;
            _color_palette[i](0) = std::clamp(_color_palette[i](0), 0.0f, 1.0f);
            _color_palette[i](1) = std::clamp(_color_palette[i](1), 0.0f, 1.0f);
            _color_palette[i](2) = std::clamp(_color_palette[i](2), 0.0f, 1.0f);
        }
    }
}

Eigen::Array3f DebugRenderer::_render_pixel(const Scene& scene,
                                            RenderData& data,
                                            int x,
                                            int y,
                                            int& num_rays)
{
    Eigen::Array3f color = Eigen::Array3f::Zero();
    auto rayhit = scene.camera()->gen_ray((x + 0.0f) / data.width,
                                          (y + 0.0f) / data.height);
    RTCIntersectContext context;
    rtcInitIntersectContext(&context);
    rtcIntersect1(scene.raw(), &context, &rayhit);
    if (rayhit.hit.geomID != RTC_INVALID_GEOMETRY_ID) {
        if (rayhit.hit.instID[0] != RTC_INVALID_GEOMETRY_ID) {
            rayhit.hit.geomID = rayhit.hit.instID[0];
        }
        switch (_mode) {
        case RENDER_MODE_NORMAL: color = _render_normal(rayhit); break;
        default:
        case RENDER_MODE_ID: color = _render_id(rayhit); break;
        }
    }
    ++num_rays;
    return color;
}

Eigen::Array3f DebugRenderer::_render_id(RTCRayHit& rayhit)
{

    return _color_palette[rayhit.hit.geomID % _color_palette.size()];
}

Eigen::Array3f DebugRenderer::_render_normal(RTCRayHit& rayhit)
{
    return get_hitnormal(rayhit).normalized().array() * 0.5f +
           Eigen::Array3f(0.5f, 0.5f, 0.5f);
}

} // namespace yart
