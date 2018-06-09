#include <yart/texture/image.h>
#include <algorithm>
#include "../util/macro.h"

namespace yart
{

Eigen::Array3f ImageTexture::value(float u,
                                   float v,
                                   const Eigen::Vector3f& p) const
{
    YUNUSED(p);
    auto i = std::clamp(static_cast<unsigned>(u * width), 0u, width - 1);
    auto j =
        std::clamp(static_cast<unsigned>((1.0f - v) * height), 0u, height - 1);
    auto r = pixels[3 * i + 3 * width * j] / 255.0f;
    auto g = pixels[3 * i + 3 * width * j + 1] / 255.0f;
    auto b = pixels[3 * i + 3 * width * j + 2] / 255.0f;
    return Eigen::Array3f(r, g, b);
}

} // namespace yart
