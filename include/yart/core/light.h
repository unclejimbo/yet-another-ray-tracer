#pragma once

#include <yart/core/common.h>
#include <embree3/rtcore.h>
#include <Eigen/Core>

namespace yart
{

class Scene;

class YART_API Light
{
public:
    virtual ~Light() = default;
    virtual bool cast_shadow_ray(const Eigen::Vector3f& target,
                                 RTCRay& shadow_ray,
                                 Eigen::Array3f& le) const = 0;
};

} // namespace yart
