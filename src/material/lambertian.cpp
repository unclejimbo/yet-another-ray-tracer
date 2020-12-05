#define _USE_MATH_DEFINES
#include <yart/material/lambertian.h>
#include <yart/core/local_frame.h>
#include <cmath>
#include "../util/rayhit.h"

namespace yart
{

Eigen::Vector3f Lambertian::sample(const RTCRayHit& rayhit, float& pdf) const
{
    LocalFrame frame(get_hitnormal(rayhit));
    return frame.local(Material::_sampler.cosine_weighted_on_hemisphere(&pdf));
}

Eigen::Array3f Lambertian::eval(const RTCRayHit& rayhit,
                                const Eigen::Vector3f& wi) const
{
    auto hitpt = get_hitpt(rayhit);
    auto normal = get_hitnormal(rayhit).normalized();
    auto cos_theta = std::abs(wi.normalized().dot(normal));
    return texture->value(rayhit.hit.u, rayhit.hit.v, hitpt) * cos_theta / M_PI;
}

} // namespace yart
