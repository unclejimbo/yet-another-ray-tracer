#pragma once

#include <yart/core/common.h>
#include <embree3/rtcore.h>
#include <Eigen/Dense>

namespace yart
{

class YART_API Material
{
public:
    virtual ~Material() = default;
    virtual bool scatter(const RTCRayHit& rayhit,
                         Eigen::Vector3f& rayout,
                         Eigen::Array3f& attenuation) const = 0;
};

} // namespace yart
