#include <yart/core/scene.h>
#include <yart/core/device.h>
#include <yart/core/geometry.h>
#include <yart/core/material.h>
#include <yart/core/camera.h>
#include <random>
#include <algorithm>
#include "../util/assert.h"
#include "../util/rayhit.h"

namespace yart
{

Scene::Scene(const Device& device) : _need_commit(true)
{
    _raw = rtcNewScene(device._raw);
}

Scene::~Scene()
{
    rtcReleaseScene(_raw);
}

void Scene::add(Geometry& geom, Material& mat)
{
    geom._id = rtcAttachGeometry(_raw, geom._raw);
    _mats.push_back(&mat);
    _need_commit = true;
}

void Scene::render(const Camera& camera,
                   unsigned char* pixels,
                   unsigned width,
                   unsigned height,
                   unsigned samples,
                   bool interleaved,
                   unsigned depth)
{
    if (_need_commit) {
        rtcCommitScene(_raw);
        _need_commit = false;
    }

    RTCIntersectContext context;
    rtcInitIntersectContext(&context);

    std::random_device rd;
    std::minstd_rand rd_gen(rd());
    std::uniform_real_distribution<> rd_number(0.0f, 1.0f);

#pragma omp parallel for
    for (unsigned y = 0; y < height; ++y) {
#pragma omp parallel for
        for (unsigned x = 0; x < width; ++x) {
            Eigen::Array3f irradiance(0.0f, 0.0f, 0.0f);
            for (unsigned s = 0; s < samples; ++s) {
                auto u = static_cast<float>(x + rd_number(rd_gen)) / width;
                auto v = static_cast<float>(y + rd_number(rd_gen)) / height;
                auto rayhit = camera.gen_ray(u, v);
                irradiance += _path_tracing(context, rayhit, depth);
            }

            irradiance /= samples;
            const float gamma = 1.0f / 2.2f;
            irradiance = irradiance.pow(gamma);
            irradiance *= 255.0f;
            auto r = std::min(static_cast<int>(irradiance(0)), 255);
            auto g = std::min(static_cast<int>(irradiance(1)), 255);
            auto b = std::min(static_cast<int>(irradiance(2)), 255);
            if (interleaved) {
                pixels[3 * ((height - y - 1) * width + x) + 0] =
                    static_cast<unsigned char>(r);
                pixels[3 * ((height - y - 1) * width + x) + 1] =
                    static_cast<unsigned char>(g);
                pixels[3 * ((height - y - 1) * width + x) + 2] =
                    static_cast<unsigned char>(b);
            }
            else {
                pixels[(height - y - 1) * width + x] =
                    static_cast<unsigned char>(r);
                pixels[width * height + (height - y - 1) * width + x] =
                    static_cast<unsigned char>(g);
                pixels[2 * width * height + (height - y - 1) * width + x] =
                    static_cast<unsigned char>(b);
            }
        }
    }
}

Eigen::Array3f Scene::_path_tracing(RTCIntersectContext& context,
                                    RTCRayHit& rayhit,
                                    unsigned depth)
{
    rtcIntersect1(_raw, &context, &rayhit);
    if (rayhit.hit.geomID != RTC_INVALID_GEOMETRY_ID) {
        // // Debug: visualize normals
        // Eigen::Vector3f color(
        //     rayhit.hit.Ng_x, rayhit.hit.Ng_y, rayhit.hit.Ng_z);
        // color += Eigen::Vector3f::Ones();
        // color *= 0.5f;
        // return color;

        auto raydir = get_raydir(rayhit);
        auto hitpt = get_hitpt(rayhit);
        auto normal = get_hitnormal(rayhit);
        Eigen::Vector3f scattered;
        Eigen::Array3f attenuation;
        if (depth > 0 && _mats[rayhit.hit.geomID]->scatter(
                             raydir, hitpt, normal, scattered, attenuation)) {
            auto secondary = make_rayhit(hitpt, scattered, 0.0001f);
            return attenuation * _path_tracing(context, secondary, depth - 1);
        }
        else {
            return Eigen::Vector3f::Zero();
        }
    }
    else { // background
        auto raydir = get_raydir(rayhit).normalized();
        float t = 0.5f * (raydir(1) + 1.0f);
        return (1.0f - t) * Eigen::Vector3f::Ones() +
               t * Eigen::Vector3f(0.5f, 0.7f, 1.0f);
    }
}

} // namespace yart
