#pragma once

#include <yart/core/geometry.h>
#include <Eigen/Core>

namespace yart
{

class YART_API BoxData : public GeometryData
{
public:
    BoxData() = default;
    BoxData(const Eigen::Vector3f& min_corner,
            const Eigen::Vector3f& max_corner);
    Eigen::Vector3f min_corner() const { return _p0; }
    Eigen::Vector3f max_corner() const { return _p1; }
    Eigen::Vector3f x() const
    {
        return Eigen::Vector3f(_p1(0) - _p0(0), 0.0f, 0.0f);
    }
    Eigen::Vector3f y() const
    {
        return Eigen::Vector3f(0.0f, _p1(1) - _p0(1), 0.0f);
    }
    Eigen::Vector3f z() const
    {
        return Eigen::Vector3f(0.0f, 0.0f, _p1(2) - _p0(2));
    }
    void bounds(const RTCBoundsFunctionArguments* args) override;
    void intersect(const RTCIntersectFunctionNArguments* args) override;
    void occluded(const RTCOccludedFunctionNArguments* args) override;

private:
    Eigen::Vector3f _p0;
    Eigen::Vector3f _p1;
};

class YART_API Box : public Geometry
{
public:
    Box(const Device& device,
        const Eigen::Vector3f& min_corner,
        const Eigen::Vector3f& max_corner);
};

} // namespace yart
