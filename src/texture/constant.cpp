#include <yart/texture/constant.h>
#include "../util/macro.h"

namespace yart
{

Eigen::Array3f ConstantTexture::value(float u,
                                      float v,
                                      const Eigen::Vector3f& p) const
{
    YUNUSED(u);
    YUNUSED(v);
    YUNUSED(p);
    return color;
}

} // namespace yart
