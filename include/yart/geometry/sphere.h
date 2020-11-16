#pragma once

#include <yart/core/geometry.h>
#include <Eigen/Core>

namespace yart
{

class YART_API SphereData
{
public:
    static void bounds(const RTCBoundsFunctionArguments* args);
    static void intersect(const RTCIntersectFunctionNArguments* args);

public:
    SphereData() = default;
    SphereData(float radius, const Eigen::Vector3f& center);
    float radius() const { return _radius; }
    Eigen::Vector3f center() const { return _center; }

private:
    float _radius;
    Eigen::Vector3f _center;
};

class YART_API Sphere : public Geometry
{
public:
    Sphere(const Device& device, float radius, const Eigen::Vector3f& center);

private:
    SphereData _data;
};

} // namespace yart
