#include <yart/core/material.h>

namespace yart
{

Sampler Material::_sampler;

Eigen::Vector3f Material::sample(const RTCRayHit& rayhit, float& pdf) const
{
    return Eigen::Vector3f::Zero();
}

Eigen::Array3f Material::eval(const RTCRayHit& rayhit,
                              const Eigen::Vector3f& wi) const
{
    return Eigen::Array3f::Zero();
}

} // namespace yart
