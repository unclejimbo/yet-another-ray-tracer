#include <yart/geometry/box.h>
#include <yart/geometry/plane.h>
#include "../util/macro.h"
#include "../util/rayhit.h"

namespace yart
{

void BoxData::bounds(const RTCBoundsFunctionArguments* args)
{
    auto bounds_o = args->bounds_o;
    auto box = reinterpret_cast<BoxData*>(args->geometryUserPtr);
    const auto& lb = box->min_corner();
    const auto& rt = box->max_corner();
    bounds_o->lower_x = lb(0);
    bounds_o->lower_y = lb(1);
    bounds_o->lower_z = lb(2);
    bounds_o->upper_x = rt(0);
    bounds_o->upper_y = rt(1);
    bounds_o->upper_z = rt(2);
}

void BoxData::intersect(const RTCIntersectFunctionNArguments* args)
{
    YASSERT(args->N == 1);

    auto box = reinterpret_cast<BoxData*>(args->geometryUserPtr);
    std::vector<PlaneData> planes;
    planes.emplace_back(box->min_corner(), box->z(), box->y());   // left
    planes.emplace_back(box->min_corner(), box->x(), box->z());   // bottom
    planes.emplace_back(box->min_corner(), box->y(), box->x());   // back
    planes.emplace_back(box->max_corner(), -box->y(), -box->z()); // right
    planes.emplace_back(box->max_corner(), -box->z(), -box->x()); // top
    planes.emplace_back(box->max_corner(), -box->x(), -box->y()); // front
    for (auto& plane : planes) {
        RTCIntersectFunctionNArguments pargs;
        pargs.valid = args->valid;
        pargs.N = args->N;
        pargs.context = args->context;
        pargs.rayhit = args->rayhit;
        pargs.primID = args->primID;
        pargs.geomID = args->geomID;
        pargs.geometryUserPtr = &plane;
        plane.intersect(&pargs);
    }
}

BoxData::BoxData(const Eigen::Vector3f& min_corner,
                 const Eigen::Vector3f& max_corner)
    : _p0(min_corner), _p1(max_corner)
{}

Box::Box(const Device& device,
         const Eigen::Vector3f& min_corner,
         const Eigen::Vector3f& max_corner)
    : Geometry(device, RTC_GEOMETRY_TYPE_USER), _data(min_corner, max_corner)
{
    rtcSetGeometryUserPrimitiveCount(this->_raw, 1);
    rtcSetGeometryUserData(this->_raw, &_data);
    rtcSetGeometryBoundsFunction(this->_raw, &BoxData::bounds, nullptr);
    rtcSetGeometryIntersectFunction(this->_raw, &BoxData::intersect);
    rtcCommitGeometry(this->_raw);
}

} // namespace yart
