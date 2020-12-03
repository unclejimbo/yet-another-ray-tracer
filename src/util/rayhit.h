#pragma once

#include <embree3/rtcore.h>
#include <Eigen/Core>
#include <limits>
#include <cmath>
#include <optional>

namespace yart
{

inline Eigen::Vector3f get_rayorg(const RTCRay& ray)
{
    return Eigen::Vector3f(ray.org_x, ray.org_y, ray.org_z);
}

inline Eigen::Vector3f get_rayorg(const RTCRayHit& rayhit)
{
    return get_rayorg(rayhit.ray);
}

inline Eigen::Vector3f get_raydir(const RTCRay& ray)
{
    return Eigen::Vector3f(ray.dir_x, ray.dir_y, ray.dir_z);
}

inline Eigen::Vector3f get_raydir(const RTCRayHit& rayhit)
{
    return get_raydir(rayhit.ray);
}

inline Eigen::Vector3f get_hitpt(const RTCRayHit& rayhit)
{
    auto org = get_rayorg(rayhit);
    auto dir = get_raydir(rayhit);
    return org + dir * rayhit.ray.tfar;
}

inline Eigen::Vector3f get_hitnormal(const RTCRayHit& rayhit)
{
    return Eigen::Vector3f(rayhit.hit.Ng_x, rayhit.hit.Ng_y, rayhit.hit.Ng_z);
}

inline Eigen::Vector3f transform_hitnormal(
    RTCRayHit& rayhit,
    const Eigen::Matrix4f& model_to_world)
{
    Eigen::Vector3f n = get_hitnormal(rayhit);
    n = model_to_world.topLeftCorner(3, 3).inverse().transpose() * n;
    n.normalize();
    return n;
}

inline RTCRay make_ray(const Eigen::Vector3f& rayorg,
                       const Eigen::Vector3f& raydir,
                       float tnear = 0.0f,
                       float tfar = std::numeric_limits<float>::max())
{
    RTCRay ray;
    ray.org_x = rayorg(0);
    ray.org_y = rayorg(1);
    ray.org_z = rayorg(2);
    ray.dir_x = raydir(0);
    ray.dir_y = raydir(1);
    ray.dir_z = raydir(2);
    ray.tnear = tnear;
    ray.tfar = tfar;
    return ray;
}

inline RTCRayHit make_rayhit(const Eigen::Vector3f& rayorg,
                             const Eigen::Vector3f& raydir,
                             float tnear = 0.0f,
                             float tfar = std::numeric_limits<float>::max())
{
    RTCRayHit rayhit;
    rayhit.ray.org_x = rayorg(0);
    rayhit.ray.org_y = rayorg(1);
    rayhit.ray.org_z = rayorg(2);
    rayhit.ray.dir_x = raydir(0);
    rayhit.ray.dir_y = raydir(1);
    rayhit.ray.dir_z = raydir(2);
    rayhit.ray.tnear = tnear;
    rayhit.ray.tfar = tfar;
    rayhit.hit.geomID = RTC_INVALID_GEOMETRY_ID;
    return rayhit;
}

inline Eigen::Vector3f reflect(const Eigen::Vector3f& in,
                               const Eigen::Vector3f& n)
{
    return in - 2.0f * in.dot(n) * n;
}

inline std::optional<Eigen::Vector3f> refract(const Eigen::Vector3f& in,
                                              const Eigen::Vector3f& n,
                                              float ni_over_nt)
{
    auto uin = in.normalized();
    auto cosi = uin.dot(n);
    auto cos2t = 1.0f - (1.0f - cosi * cosi) * ni_over_nt * ni_over_nt;
    if (cos2t > 0.0f) {
        Eigen::Vector3f refracted =
            (uin - n * cosi) * ni_over_nt - n * std::sqrt(cos2t);
        return std::make_optional(refracted);
    }
    else { // total reflection
        return std::nullopt;
    }
}

} // namespace yart
