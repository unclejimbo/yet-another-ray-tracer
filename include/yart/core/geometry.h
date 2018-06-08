#pragma once

#include <yart/core/common.h>
#include <embree3/rtcore.h>

namespace yart
{

class Device;

class YART_API Geometry
{
    friend class Scene;

public:
    Geometry(const Device& device, RTCGeometryType type);
    ~Geometry();
    unsigned id() const;

protected:
    RTCGeometry _raw;
    unsigned _id;
};

} // namespace yart
