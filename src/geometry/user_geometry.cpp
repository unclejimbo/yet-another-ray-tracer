#include <yart/geometry/user_geometry.h>

namespace yart
{

static void bounds(const RTCBoundsFunctionArguments* args)
{
    auto data = reinterpret_cast<UserGeometry*>(args->geometryUserPtr);
    data->bounds(args);
}

static void intersect(const RTCIntersectFunctionNArguments* args)
{
    auto data = reinterpret_cast<UserGeometry*>(args->geometryUserPtr);
    data->intersect(args);
}

static void occluded(const RTCOccludedFunctionNArguments* args)
{
    auto data = reinterpret_cast<UserGeometry*>(args->geometryUserPtr);
    data->occluded(args);
}

UserGeometry::UserGeometry(const Device& device, int prim_count)
    : Geometry(device, RTC_GEOMETRY_TYPE_USER)
{
    rtcSetGeometryUserPrimitiveCount(this->_raw, prim_count);
    rtcSetGeometryUserData(this->_raw, this);
    rtcSetGeometryBoundsFunction(this->_raw, &yart::bounds, nullptr);
    rtcSetGeometryIntersectFunction(this->_raw, &yart::intersect);
    rtcSetGeometryOccludedFunction(this->_raw, &yart::occluded);
    rtcCommitGeometry(this->_raw);
}

} // namespace yart
