#include <yart/geometry/box.h>
#include <yart/geometry/plane.h>
#include "../util/macro.h"
#include "../util/rayhit.h"

namespace yart
{

BoxData::BoxData(const Eigen::Vector3f& min_corner,
                 const Eigen::Vector3f& max_corner)
    : _p0(min_corner), _p1(max_corner)
{}

void BoxData::bounds(const RTCBoundsFunctionArguments* args)
{
    auto bounds_o = args->bounds_o;
    const auto& lb = this->min_corner();
    const auto& rt = this->max_corner();
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

    std::vector<PlaneData> planes;
    planes.emplace_back(this->min_corner(), this->z(), this->y());   // left
    planes.emplace_back(this->min_corner(), this->x(), this->z());   // bottom
    planes.emplace_back(this->min_corner(), this->y(), this->x());   // back
    planes.emplace_back(this->max_corner(), -this->y(), -this->z()); // right
    planes.emplace_back(this->max_corner(), -this->z(), -this->x()); // top
    planes.emplace_back(this->max_corner(), -this->x(), -this->y()); // front
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

void BoxData::occluded(const RTCOccludedFunctionNArguments* args)
{
    YASSERT(args->N == 1);

    std::vector<PlaneData> planes;
    planes.emplace_back(this->min_corner(), this->z(), this->y());   // left
    planes.emplace_back(this->min_corner(), this->x(), this->z());   // bottom
    planes.emplace_back(this->min_corner(), this->y(), this->x());   // back
    planes.emplace_back(this->max_corner(), -this->y(), -this->z()); // right
    planes.emplace_back(this->max_corner(), -this->z(), -this->x()); // top
    planes.emplace_back(this->max_corner(), -this->x(), -this->y()); // front
    for (auto& plane : planes) {
        RTCOccludedFunctionNArguments pargs;
        pargs.valid = args->valid;
        pargs.N = args->N;
        pargs.context = args->context;
        pargs.ray = args->ray;
        pargs.primID = args->primID;
        pargs.geomID = args->geomID;
        pargs.geometryUserPtr = &plane;
        plane.occluded(&pargs);
    }
}

Box::Box(const Device& device,
         const Eigen::Vector3f& min_corner,
         const Eigen::Vector3f& max_corner)
    : UserGeometry(device)
{
    _data = std::make_unique<BoxData>(min_corner, max_corner);
}

LocalGeometry Box::sample() const
{
    // TODO:
    return LocalGeometry();
}

} // namespace yart
