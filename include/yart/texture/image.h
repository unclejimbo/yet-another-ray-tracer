#pragma once

#include <yart/core/texture.h>

namespace yart
{

class YART_API ImageTexture : public Texture
{
public:
    ImageTexture() = default;
    ImageTexture(unsigned char* pixels, unsigned width, unsigned height)
        : pixels(pixels), width(width), height(height)
    {}
    Eigen::Array3f value(float u,
                         float v,
                         const Eigen::Vector3f& p) const override;

public:
    unsigned char* pixels;
    unsigned width;
    unsigned height;
};

} // namespace yart
