#pragma once

#include <yart/core/material.h>
#include <yart/core/texture.h>

namespace yart
{

class YART_API Lambertian : public Material
{
public:
    explicit Lambertian(Texture* texture) : texture(texture) {}
    Eigen::Vector3f sample(const RTCRayHit& rayhit, float& pdf) const override;
    Eigen::Array3f eval(const RTCRayHit& rayhit,
                        const Eigen::Vector3f& wi) const override;

public:
    Texture* texture;
};

} // namespace yart
