#include <yart/material/diffuse_light.h>

namespace yart
{

bool DiffuseLight::scatter(const RTCRayHit& rayhit,
                           Eigen::Vector3f& rayout,
                           Eigen::Array3f& attenuation) const
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
