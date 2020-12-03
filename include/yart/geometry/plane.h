#pragma once

#include <yart/core/geometry.h>
#include <Eigen/Core>

namespace yart
{

class YART_API PlaneData : public GeometryData
{

public:
    PlaneData() = default;
    PlaneData(const Eigen::Vector3f& corner,
              const Eigen::Vector3f& u,
              const Eigen::Vector3f v);
    Eigen::Vector3f corner() const { return _corner; };
    Eigen::Vector3f u() const { return _u; }
    Eigen::Vector3f v() const { return _v; }
    Eigen::Vector3f udir() const { return _u.normalized(); }
    Eigen::Vector3f vdir() const { return _v.normalized(); }
    Eigen::Vector3f n() const { return _n; }
    float ulen() const { return _u.norm(); }
    float vlen() const { return _v.norm(); }
    void bounds(const RTCBoundsFunctionArguments* args) override;
    void intersect(const RTCIntersectFunctionNArguments* args) override;
    void occluded(const RTCOccludedFunctionNArguments* args) override;

private:
    Eigen::Vector3f _corner;
    Eigen::Vector3f _u;
    Eigen::Vector3f _v;
    Eigen::Vector3f _n;
};

class YART_API Plane : public Geometry
{
public:
    Plane(const Device& device,
          const Eigen::Vector3f& corner,
          const Eigen::Vector3f& u,
          const Eigen::Vector3f& v);
};

} // namespace yart
