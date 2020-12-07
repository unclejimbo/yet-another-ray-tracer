#include <yart/material/diffuse_light.h>
#include "../util/rayhit.h"

namespace yart
{

Eigen::Array3f DiffuseLight::eval(const RTCRayHit& rayhit,
                                  const Eigen::Vector3f& wi) const
{
    return emit->value(rayhit.hit.u, rayhit.hit.v, get_hitpt(rayhit));
}

Eigen::Array3f DiffuseLight::eval(float u, float v) const
{
    Eigen::Vector3f dumb;
    return emit->value(u, v, dumb);
}

} // namespace yart
