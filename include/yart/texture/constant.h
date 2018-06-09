#pragma once

#include <yart/core/texture.h>

namespace yart
{

class YART_API ConstantTexture : public Texture
{
public:
    ConstantTexture() = default;
    explicit ConstantTexture(const Eigen::Array3f& color) : color(color) {}
    ConstantTexture(float r, float g, float b) : color(r, g, b) {}
    Eigen::Array3f value(float u,
                         float v,
                         const Eigen::Vector3f& p) const override;

public:
    Eigen::Array3f color = Eigen::Array3f::Ones();
};

} // namespace yart
