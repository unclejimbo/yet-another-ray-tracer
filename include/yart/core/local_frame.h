#pragma once

#include <yart/core/common.h>
#include <Eigen/Core>

namespace yart
{

class YART_API LocalFrame
{
public:
    LocalFrame() = default;
    LocalFrame(const Eigen::Vector3f& v);
    Eigen::Vector3f u() const { return _u; }
    Eigen::Vector3f v() const { return _v; }
    Eigen::Vector3f w() const { return _w; }
    Eigen::Vector3f local(const Eigen::Vector3f world) const
    {
        return _u * world(0) + _v * world(1) + _w * world(2);
    }
    Eigen::Vector3f local(float x, float y, float z)
    {
        return _u * x + _v * y + _w * z;
    }

private:
    Eigen::Vector3f _u;
    Eigen::Vector3f _v;
    Eigen::Vector3f _w;
};

} // namespace yart
