#include <yart/renderer/pathtracer.h>
#include <yart/core/camera.h>
#include <yart/core/material.h>
#include <yart/core/scene.h>
#include <yart/geometry/instance.h>
#include "../util/rayhit.h"

namespace yart
{

Eigen::Array3f PathTracer::_render_pixel(const Scene& scene,
                                         RenderData& data,
                                         int x,
                                         int y,
                                         int& num_rays)
{
    Eigen::Array3f irradiance = Eigen::Array3f::Zero();

    std::random_device rd;
    std::minstd_rand rd_gen(rd());
    std::uniform_real_distribution<> rd_number(0.0f, 1.0f);

    // random sampling
    for (int s = 0; s < _samples; ++s) {
        auto u = static_cast<float>(x + rd_number(rd_gen)) / data.width;
        auto v = static_cast<float>(y + rd_number(rd_gen)) / data.height;
        auto rayhit = scene.camera()->gen_ray(u, v);
        irradiance += _path_tracing(scene, rayhit, num_rays);
    }
    irradiance /= _samples;

    // gamma correction
    irradiance = irradiance.pow(GAMMA);

    return irradiance;
}

Eigen::Array3f PathTracer::_path_tracing(const Scene& scene,
                                         RTCRayHit& rayhit,
                                         int& num_rays)
{
    RTCIntersectContext context;
    rtcInitIntersectContext(&context);

    Eigen::Array3f irradiance = Eigen::Array3f::Zero();
    Eigen::Array3f throughput = Eigen::Array3f::Ones();

    for (int i = 0; i < _depth; ++i) {
        ++num_rays;
        rtcIntersect1(scene.raw(), &context, &rayhit);
        if (rayhit.hit.geomID != RTC_INVALID_GEOMETRY_ID) {
            // deal with geometry instancing
            if (rayhit.hit.instID[0] != RTC_INVALID_GEOMETRY_ID) {
                rayhit.hit.geomID = rayhit.hit.instID[0];
                auto instance = reinterpret_cast<Instance*>(
                    scene.geometries()[rayhit.hit.geomID]);
                Eigen::Vector3f normal =
                    transform_hitnormal(rayhit, instance->matrix());
                rayhit.hit.Ng_x = normal(0);
                rayhit.hit.Ng_y = normal(1);
                rayhit.hit.Ng_z = normal(2);
            }
            auto hitpt = get_hitpt(rayhit);
            auto hitmat = scene.materials()[rayhit.hit.geomID];

            if (hitmat->is_emissive()) {
                Eigen::Vector3f wi; // dumb
                irradiance = throughput * hitmat->eval(rayhit, wi);
                break;
            }
            else {
                float pdf;
                auto wi = hitmat->sample(rayhit, pdf);
                if (wi != Eigen::Vector3f::Zero()) {
                    throughput *= hitmat->eval(rayhit, wi) / pdf;
                    rayhit = make_rayhit(hitpt, wi, 1e-4f);
                }
                else {
                    break;
                }
            }
        }
        else {
            irradiance = throughput * scene.background().array();
            break;
        }
    }

    return irradiance;
}

} // namespace yart
