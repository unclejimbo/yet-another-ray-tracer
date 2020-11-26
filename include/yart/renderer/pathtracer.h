#pragma once

#include <yart/core/common.h>
#include <yart/core/renderer.h>
#include <embree3/rtcore.h>

namespace yart
{

class YART_API PathTracer : public Renderer
{
    static constexpr const float GAMMA = 1.0f / 2.2f;

public:
    PathTracer() {}
    PathTracer(int samples, int depth) : _samples(samples), _depth(depth) {}
    ~PathTracer() {}
    int samples() const { return _samples; }
    int depth() const { return _depth; }
    void set_samples(int samples) { _samples = samples; }
    void set_depth(int depth) { _depth = depth; }

protected:
    Eigen::Array3f _render_pixel(const Scene& scene,
                                 RenderData& data,
                                 int x,
                                 int y,
                                 int& num_rays) override;

private:
    Eigen::Array3f _path_tracing(const Scene& scene,
                                 RTCRayHit& rayhit,
                                 int& num_rays);

private:
    int _samples = 1;
    int _depth = 100;
};

} // namespace yart
