#pragma once

#include <yart/core/common.h>
#include <Eigen/Core>

namespace yart
{

class YART_API Texture
{
public:
    virtual ~Texture() = default;
    virtual Eigen::Array3f value(float u,
                                 float v,
                                 const Eigen::Vector3f& p) const = 0;
};

} // namespace yart
