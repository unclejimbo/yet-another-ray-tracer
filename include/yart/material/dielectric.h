#pragma once

#include <yart/core/material.h>

namespace yart
{

class YART_API Dielectric : public Material
{
public:
    explicit Dielectric(float rindex) : rindex(rindex) {}
    Eigen::Vector3f sample(const RTCRayHit& rayhit, float& pdf) const override;
    Eigen::Array3f eval(const RTCRayHit& rayhit,
                        const Eigen::Vector3f& wi) const override;

public:
    float rindex;
};

} // namespace yart
