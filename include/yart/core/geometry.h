#pragma once

#include <yart/core/common.h>
#include <embree3/rtcore.h>
#include <memory>

namespace yart
{

class Device;

class YART_API Geometry
{
public:
    Geometry(const Device& device, RTCGeometryType type);
    ~Geometry();
    RTCGeometry raw() const { return _raw; }

protected:
    RTCGeometry _raw;
};

} // namespace yart
