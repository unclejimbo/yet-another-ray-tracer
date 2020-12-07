#pragma once

#include <yart/geometry/user_geometry.h>
#include <Eigen/Core>

namespace yart
{

class YART_API PlaneData : public UserGeometryData
{

public:
    PlaneData() = default;
    PlaneData(const Eigen::Vector3f& corner,
              const Eigen::Vector3f& u,
              const Eigen::Vector3f v);
    void bounds(const RTCBoundsFunctionArguments* args) override;
    void intersect(const RTCIntersectFunctionNArguments* args) override;
    void occluded(const RTCOccludedFunctionNArguments* args) override;
    Eigen::Vector3f corner() const { return _corner; };
    Eigen::Vector3f u() const { return _u; }
    Eigen::Vector3f v() const { return _v; }
    Eigen::Vector3f udir() const { return _u.normalized(); }
    Eigen::Vector3f vdir() const { return _v.normalized(); }
    Eigen::Vector3f n() const { return _n; }
    float ulen() const { return _u.norm(); }
    float vlen() const { return _v.norm(); }

private:
    Eigen::Vector3f _corner;
    Eigen::Vector3f _u;
    Eigen::Vector3f _v;
    Eigen::Vector3f _n;
};

class YART_API Plane : public UserGeometry
{
public:
    Plane(const Device& device,
          const Eigen::Vector3f& corner,
          const Eigen::Vector3f& u,
          const Eigen::Vector3f& v);
    LocalGeometry sample() const override;
};

} // namespace yart
