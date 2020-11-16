#include <yart/material/diffuse_light.h>

namespace yart
{

bool DiffuseLight::scatter(const RTCRayHit&,
                           Eigen::Vector3f&,
                           Eigen::Array3f&) const
{
    return false;
}

Eigen::Vector3f DiffuseLight::emitted(float u,
                                      float v,
                                      const Eigen::Vector3f& p) const
{
    return emit->value(u, v, p);
}

} // namespace yart
