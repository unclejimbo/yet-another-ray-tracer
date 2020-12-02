#pragma once

#include <yart/core/common.h>
#include <yart/core/sampler.h>
#include <embree3/rtcore.h>
#include <Eigen/Core>

namespace yart
{

class YART_API Material
{
public:
    virtual ~Material() = default;
    virtual Eigen::Vector3f sample(const RTCRayHit& rayhit, float& pdf) const;
    virtual Eigen::Array3f eval(const RTCRayHit& rayhit,
                                const Eigen::Vector3f& wi) const;
    virtual bool is_emissive() const { return false; }

protected:
    static Sampler _sampler;
};

} // namespace yart
