#include <yart/core/local_frame.h>
#include <Eigen/Geometry>
#include <cmath>

namespace yart
{

LocalFrame::LocalFrame(const Eigen::Vector3f& v)
{
    _v = v.normalized();
    if (std::abs(v(0)) < 0.9f) {
        _w = Eigen::Vector3f(1.0f, 0.0f, 0.0f).cross(_v).normalized();
    }
    else {
        _w = Eigen::Vector3f(0.0f, 0.0f, 1.0f).cross(_v).normalized();
    }
    _u = _v.cross(_w);
}

} // namespace yart
