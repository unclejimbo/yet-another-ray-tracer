#include <yart/texture/constant.h>

namespace yart
{

Eigen::Array3f ConstantTexture::value(float u,
                                      float v,
                                      const Eigen::Vector3f& p) const
{
    return color;
}

} // namespace yart
