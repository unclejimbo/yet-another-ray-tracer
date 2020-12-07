#pragma once

#include <yart/geometry/user_geometry.h>
#include <Eigen/Core>

namespace yart
{

class YART_API SphereData : public UserGeometryData
{
public:
    SphereData() = default;
    SphereData(float radius, const Eigen::Vector3f& center);
    void bounds(const RTCBoundsFunctionArguments* args) override;
    void intersect(const RTCIntersectFunctionNArguments* args) override;
    void occluded(const RTCOccludedFunctionNArguments* args) override;
    void initialize(float radius, Eigen::Vector3f center);
    float radius() const { return _radius; }
    Eigen::Vector3f center() const { return _center; }

private:
    float _radius;
    Eigen::Vector3f _center;
};

class YART_API Sphere : public UserGeometry
{
public:
    Sphere(const Device& device, float radius, const Eigen::Vector3f& center);
    LocalGeometry sample() const override;
};

} // namespace yart
