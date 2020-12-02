#include <yart/core/material.h>

namespace yart
{

Sampler Material::_sampler;

Eigen::Vector3f Material::emitted(float, float, const Eigen::Vector3f&) const
{
    return Eigen::Vector3f::Zero();
}

} // namespace yart
