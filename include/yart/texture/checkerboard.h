#pragma once

#include <yart/core/texture.h>

namespace yart
{

class YART_API CheckerBoardTexture : public Texture
{
public:
    CheckerBoardTexture() = default;
    CheckerBoardTexture(Texture* t0, Texture* t1) : odd(t0), even(t1) {}
    Eigen::Array3f value(float u,
                         float v,
                         const Eigen::Vector3f& p) const override;

public:
    Texture* odd;
    Texture* even;
};

} // namespace yart
