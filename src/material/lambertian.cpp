#include <yart/material/lambertian.h>
#include "../util/rayhit.h"

namespace yart
{

Eigen::Vector3f Lambertian::sample(const RTCRayHit& rayhit, float& pdf) const
{
    pdf = 1.0f;
    auto hitpt = get_hitpt(rayhit);
    auto normal = get_hitnormal(rayhit);
    Eigen::Vector3f wi =
        hitpt + normal.normalized() + Material::_sampler.uniform_in_sphere();
    wi -= hitpt;
    return wi;
}

Eigen::Array3f Lambertian::eval(const RTCRayHit& rayhit,
                                const Eigen::Vector3f& wi) const
{
    auto hitpt = get_hitpt(rayhit);
    return texture->value(rayhit.hit.u, rayhit.hit.v, hitpt);
}

} // namespace yart
