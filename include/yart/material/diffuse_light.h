#pragma once

#include <yart/core/material.h>
#include <yart/core/texture.h>

namespace yart
{

class YART_API DiffuseLight : public Material
{
public:
    explicit DiffuseLight(Texture* texture) : emit(texture) {}
    Eigen::Array3f eval(const RTCRayHit& rayhit,
                        const Eigen::Vector3f& wi) const override;
    Eigen::Array3f eval(float u, float v) const;
    bool is_emissive() const override { return true; }

public:
    Texture* emit;
};

} // namespace yart
