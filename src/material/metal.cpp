#include <yart/material/metal.h>
#include <algorithm>
#include "../util/rayhit.h"

namespace yart
{

Metal::Metal(const Eigen::Array3f& albedo, float roughness) : albedo(albedo)
{
    this->roughness = std::clamp(roughness, 0.0f, 1.0f);
}

Metal::Metal(float r, float g, float b, float roughness) : albedo(r, g, b)
{
    this->roughness = std::clamp(roughness, 0.0f, 1.0f);
}

Eigen::Vector3f Metal::sample(const RTCRayHit& rayhit, float& pdf) const
{
    pdf = 1.0f;
    auto wo = get_raydir(rayhit);
    auto normal = get_hitnormal(rayhit);
    auto wi = reflect(wo, normal.normalized());
    wi = wi + roughness * Material::_sampler.uniform_in_sphere();
    if (wi.dot(normal) > 0.0f) { return wi; }
    else {
        return Eigen::Vector3f::Zero();
    }
}

Eigen::Array3f Metal::eval(const RTCRayHit& rayhit,
                           const Eigen::Vector3f& wi) const
{
    return albedo;
}

} // namespace yart
