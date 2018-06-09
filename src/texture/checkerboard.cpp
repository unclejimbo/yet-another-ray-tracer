#include <yart/texture/checkerboard.h>
#include <cmath>

namespace yart
{

Eigen::Array3f CheckerBoardTexture::value(float u,
                                          float v,
                                          const Eigen::Vector3f& p) const
{
    auto sines = std::sin(10.0f * p.x()) * std::sin(10.0f * p.y()) *
                 std::sin(10.0f * p.z());
    if (sines < 0.0f) { return odd->value(u, v, p); }
    else {
        return even->value(u, v, p);
    }
}

} // namespace yart
