#include <yart/core/geometry.h>
#include <yart/core/device.h>

namespace yart
{

Geometry::Geometry(const Device& device, RTCGeometryType type)
{
    _raw = rtcNewGeometry(device._raw, type);
}

Geometry::~Geometry()
{
    rtcReleaseGeometry(_raw);
}

unsigned Geometry::id() const
{
    return _id;
}

} // namespace yart
