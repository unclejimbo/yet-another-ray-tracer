#include <yart/geometry/plane.h>
#include "../util/macro.h"
#include "../util/rayhit.h"
#include <Eigen/Geometry>

namespace yart
{

static void bounds(const RTCBoundsFunctionArguments* args)
{
    auto data = reinterpret_cast<PlaneData*>(args->geometryUserPtr);
    data->bounds(args);
}

static void intersect(const RTCIntersectFunctionNArguments* args)
{
    auto data = reinterpret_cast<PlaneData*>(args->geometryUserPtr);
    data->intersect(args);
}

static void occluded(const RTCOccludedFunctionNArguments* args)
{
    auto data = reinterpret_cast<PlaneData*>(args->geometryUserPtr);
    data->occluded(args);
}

PlaneData::PlaneData(const Eigen::Vector3f& corner,
                     const Eigen::Vector3f& u,
                     const Eigen::Vector3f v)
    : _corner(corner), _u(u), _v(v), _n(u.cross(v).normalized())
{}

void PlaneData::bounds(const RTCBoundsFunctionArguments* args)
{
    auto bounds_o = args->bounds_o;
    auto lb = _corner;
    auto rt = lb + _u + _v;
    bounds_o->lower_x = std::min(lb(0), rt(0));
    bounds_o->lower_y = std::min(lb(1), rt(1));
    bounds_o->lower_z = std::min(lb(2), rt(2));
    bounds_o->upper_x = std::max(lb(0), rt(0));
    bounds_o->upper_y = std::max(lb(1), rt(1));
    bounds_o->upper_z = std::max(lb(2), rt(2));
}

void PlaneData::intersect(const RTCIntersectFunctionNArguments* args)
{
    YASSERT(args->N == 1);

    auto rayhit = reinterpret_cast<RTCRayHit*>(args->rayhit);
    Eigen::Vector3f rayorg = get_rayorg(*rayhit);
    Eigen::Vector3f raydir = get_raydir(*rayhit);

    Eigen::Vector3f normal = this->n();
    auto dn = raydir.dot(normal);
    if (dn >= 0.0f) { return; } // miss

    Eigen::Vector3f planeorg = this->corner();
    auto t = ((planeorg - rayorg).dot(normal)) / dn;
    if (t > rayhit->ray.tfar || t < rayhit->ray.tnear) {
        return;
    } // out of range

    Eigen::Vector3f p = rayorg + t * raydir;
    Eigen::Vector3f pp = p - planeorg;
    auto u = pp.dot(this->udir());
    if (u < 0.0 || u > this->ulen()) { return; } // out of bound
    auto v = pp.dot(this->vdir());
    if (v < 0.0 || v > this->vlen()) { return; } // out of bound

    rayhit->ray.tfar = t;
    rayhit->hit.instID[0] = args->context->instID[0];
    rayhit->hit.geomID = args->geomID;
    rayhit->hit.primID = args->primID;
    rayhit->hit.Ng_x = normal.x();
    rayhit->hit.Ng_y = normal.y();
    rayhit->hit.Ng_z = normal.z();
    rayhit->hit.u = u / this->ulen();
    rayhit->hit.v = v / this->vlen();
}

void PlaneData::occluded(const RTCOccludedFunctionNArguments* args)
{
    YASSERT(args->N == 1);

    auto ray = reinterpret_cast<RTCRay*>(args->ray);
    auto rayorg = get_rayorg(*ray);
    auto raydir = get_raydir(*ray);

    Eigen::Vector3f normal = this->n();
    auto dn = raydir.dot(normal);
    if (dn >= 0.0f) { return; } // miss

    Eigen::Vector3f planeorg = this->corner();
    auto t = ((planeorg - rayorg).dot(normal)) / dn;
    if (t > ray->tfar || t < ray->tnear) { return; } // out of range

    Eigen::Vector3f p = rayorg + t * raydir;
    Eigen::Vector3f pp = p - planeorg;
    auto u = pp.dot(this->udir());
    if (u < 0.0 || u > this->ulen()) { return; } // out of bound
    auto v = pp.dot(this->vdir());
    if (v < 0.0 || v > this->vlen()) { return; } // out of bound

    ray->tfar = t;
}

Plane::Plane(const Device& device,
             const Eigen::Vector3f& corner,
             const Eigen::Vector3f& u,
             const Eigen::Vector3f& v)
    : Geometry(device, RTC_GEOMETRY_TYPE_USER)
{
    this->_data = std::make_unique<PlaneData>(corner, u, v);
    rtcSetGeometryUserPrimitiveCount(this->_raw, 1);
    rtcSetGeometryUserData(this->_raw, _data.get());
    rtcSetGeometryBoundsFunction(this->_raw, &bounds, nullptr);
    rtcSetGeometryIntersectFunction(this->_raw, &intersect);
    rtcSetGeometryOccludedFunction(this->_raw, &occluded);
    rtcCommitGeometry(this->_raw);
}

} // namespace yart
