#pragma once

#include <yart/core/geometry.h>
#include <Eigen/Core>

namespace yart
{

class YART_API SphereData : public GeometryData
{
public:
    SphereData() = default;
    SphereData(float radius, const Eigen::Vector3f& center);
    float radius() const { return _radius; }
    Eigen::Vector3f center() const { return _center; }
    void bounds(const RTCBoundsFunctionArguments* args) override;
    void intersect(const RTCIntersectFunctionNArguments* args) override;
    void occluded(const RTCOccludedFunctionNArguments* args) override;

private:
    float _radius;
    Eigen::Vector3f _center;
};

class YART_API Sphere : public Geometry
{
public:
    Sphere(const Device& device, float radius, const Eigen::Vector3f& center);
};

} // namespace yart
