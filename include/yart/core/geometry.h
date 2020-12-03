#pragma once

#include <yart/core/common.h>
#include <embree3/rtcore.h>
#include <memory>

namespace yart
{

class Device;

class YART_API GeometryData
{
public:
    virtual ~GeometryData() = default;
    virtual void bounds(const RTCBoundsFunctionArguments* args) = 0;
    virtual void intersect(const RTCIntersectFunctionNArguments* args) = 0;
    virtual void occluded(const RTCOccludedFunctionNArguments* args) = 0;
};

class YART_API Geometry
{
public:
    Geometry(const Device& device, RTCGeometryType type);
    ~Geometry();
    RTCGeometry raw() const { return _raw; }

protected:
    RTCGeometry _raw;
    std::unique_ptr<GeometryData> _data;
};

} // namespace yart
