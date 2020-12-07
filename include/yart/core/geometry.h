#pragma once

#include <yart/core/common.h>
#include <yart/core/sampler.h>
#include <embree3/rtcore.h>
#include <Eigen/Core>
#include <memory>

namespace yart
{

class Device;

struct LocalGeometry
{
    Eigen::Vector3f position;
    Eigen::Vector3f normal;
    float u;
    float v;
};

class YART_API Geometry
{
public:
    Geometry(const Device& device, RTCGeometryType type);
    virtual ~Geometry();
    RTCGeometry raw() const { return _raw; }
    virtual LocalGeometry sample() const = 0;

protected:
    static Sampler _sampler;

protected:
    RTCGeometry _raw;
};

} // namespace yart
