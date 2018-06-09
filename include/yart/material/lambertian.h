#pragma once

#include <yart/core/material.h>
#include <yart/core/texture.h>

namespace yart
{

class YART_API Lambertian : public Material
{
public:
    explicit Lambertian(Texture* texture) : texture(texture) {}
    bool scatter(const RTCRayHit& rayhit,
                 Eigen::Vector3f& rayout,
                 Eigen::Array3f& attenuation) const override;

public:
    Texture* texture;
};

} // namespace yart
