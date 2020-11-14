#pragma once

#include <yart/core/material.h>
#include <yart/core/texture.h>

namespace yart
{

class YART_API DiffuseLight : public Material
{
public:
    explicit DiffuseLight(Texture* texture) : emit(texture) {}
    bool scatter(const RTCRayHit& rayhit,
                 Eigen::Vector3f& rayout,
                 Eigen::Array3f& attenuation) const override;
    Eigen::Vector3f emitted(float u,
                            float v,
                            const Eigen::Vector3f& p) const override;

public:
    Texture* emit;
};

} // namespace yart
