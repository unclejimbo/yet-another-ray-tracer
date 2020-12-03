#pragma once

#include <yart/core/common.h>
#include <yart/core/renderer.h>
#include <embree3/rtcore.h>
#include <vector>

namespace yart
{

class YART_API DebugRenderer : public Renderer
{
public:
    enum RenderMode
    {
        RENDER_MODE_ID,
        RENDER_MODE_NORMAL,
    };

public:
    explicit DebugRenderer(RenderMode mode = RENDER_MODE_ID);
    ~DebugRenderer() = default;
    RenderMode render_mode() const { return _mode; }
    void set_render_mode(RenderMode mode) { _mode = mode; }
    void generate_color_palette(int num);

protected:
    Eigen::Array3f _render_pixel(const Scene& scene,
                                 RenderData& data,
                                 int x,
                                 int y,
                                 int& num_rays) override;

private:
    Eigen::Array3f _render_id(RTCRayHit& rayhit);

    Eigen::Array3f _render_normal(RTCRayHit& rayhit);

private:
    RenderMode _mode;
    std::vector<Eigen::Array3f> _color_palette;
};

} // namespace yart
