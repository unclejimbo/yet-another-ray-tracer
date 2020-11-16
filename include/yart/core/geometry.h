#pragma once

#include <yart/core/common.h>
#include <embree3/rtcore.h>
#include <memory>

namespace yart
{

class Device;

class YART_API Geometry
{
    friend class Scene;

public:
    Geometry(const Device& device, RTCGeometryType type);
    ~Geometry();

protected:
    RTCGeometry _raw;
};

} // namespace yart
