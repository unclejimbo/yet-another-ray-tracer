#pragma once

#include <yart/core/geometry.h>
#include <Eigen/Dense>

namespace yart
{

class YART_API Sphere : public Geometry
{
public:
    Sphere(const Device& device, float radius, const Eigen::Vector3f& center);
    float radius() const;
    Eigen::Vector3f center() const;

private:
    float _radius;
    Eigen::Vector3f _center;
};

} // namespace yart
