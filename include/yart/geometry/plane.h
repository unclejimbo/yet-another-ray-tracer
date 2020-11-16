#pragma once

#include <yart/core/geometry.h>
#include <Eigen/Core>

namespace yart
{

class YART_API Plane : public Geometry
{
public:
    Plane(const Device& device,
          const Eigen::Vector3f& corner,
          const Eigen::Vector3f& u,
          const Eigen::Vector3f& v);
    Eigen::Vector3f corner() const { return _corner; }
    Eigen::Vector3f normal() const { return _n; }
    Eigen::Vector3f u() const { return _u; }
    Eigen::Vector3f v() const { return _v; }
    float ulen() const { return _ulen; }
    float ulen_squared() const { return _ulen * _ulen; }
    float vlen() const { return _vlen; }
    float vlen_squared() const { return _vlen * _vlen; }

private:
    Eigen::Vector3f _corner;
    Eigen::Vector3f _u;
    Eigen::Vector3f _v;
    Eigen::Vector3f _n;
    float _ulen;
    float _vlen;
};

} // namespace yart
