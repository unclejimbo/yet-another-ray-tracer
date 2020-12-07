#include <yart/core/geometry.h>
#include <yart/core/device.h>

namespace yart
{

Sampler Geometry::_sampler;

Geometry::Geometry(const Device& device, RTCGeometryType type)
{
    _raw = rtcNewGeometry(device.raw(), type);
}

Geometry::~Geometry()
{
    rtcReleaseGeometry(_raw);
}

} // namespace yart
