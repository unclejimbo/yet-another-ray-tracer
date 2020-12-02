#pragma once

#include <yart/core/material.h>

namespace yart
{

class YART_API Metal : public Material
{
public:
    Metal(const Eigen::Array3f& albedo, float roughness);
    Metal(float r, float g, float b, float roughness);
    Eigen::Vector3f sample(const RTCRayHit& rayhit, float& pdf) const override;
    Eigen::Array3f eval(const RTCRayHit& rayhit,
                        const Eigen::Vector3f& wi) const override;

public:
    Eigen::Array3f albedo;
    float roughness;
};

} // namespace yart
