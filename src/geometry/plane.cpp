#include <yart/geometry/plane.h>
#include "../util/macro.h"
#include "../util/rayhit.h"
#include <Eigen/Geometry>

namespace yart
{

static void _plane_bounds(const RTCBoundsFunctionArguments* args)
{
    auto bounds_o = args->bounds_o;
    auto plane = reinterpret_cast<Plane*>(args->geometryUserPtr);
    auto lb = plane->corner();
    auto rt = lb + plane->u() + plane->v();
    bounds_o->lower_x = std::min(lb(0), rt(0));
    bounds_o->lower_y = std::min(lb(1), rt(1));
    bounds_o->lower_z = std::min(lb(2), rt(2));
    bounds_o->upper_x = std::max(lb(0), rt(0));
    bounds_o->upper_y = std::max(lb(1), rt(1));
    bounds_o->upper_z = std::max(lb(2), rt(2));
}

static void _plane_intersect(const RTCIntersectFunctionNArguments* args)
{
    YASSERT(args->N == 1);

    auto rayhit = reinterpret_cast<RTCRayHit*>(args->rayhit);
    Eigen::Vector3f rayorg = get_rayorg(*rayhit);
    Eigen::Vector3f raydir = get_raydir(*rayhit);

    auto plane = reinterpret_cast<Plane*>(args->geometryUserPtr);
    Eigen::Vector3f normal = plane->normal();

    auto dn = raydir.dot(normal);
    if (dn >= 0.0f) { return; } // miss

    Eigen::Vector3f planeorg = plane->corner();
    auto t = ((planeorg - rayorg).dot(normal)) / dn;
    if (t > rayhit->ray.tfar && t < rayhit->ray.tnear) {
        return;
    } // out of range

    Eigen::Vector3f p = rayorg + t * raydir;
    Eigen::Vector3f pp = p - planeorg;
    auto u = pp.dot(plane->u().normalized());
    if (u < 0.0 || u > plane->ulen()) { return; } // out of bound
    auto v = pp.dot(plane->v().normalized());
    if (v < 0.0 || v > plane->vlen()) { return; } // out of bound

    rayhit->ray.tfar = t;
    rayhit->hit.instID[0] = args->context->instID[0];
    rayhit->hit.geomID = plane->id();
    rayhit->hit.primID = args->primID;
    rayhit->hit.Ng_x = normal.x();
    rayhit->hit.Ng_y = normal.y();
    rayhit->hit.Ng_z = normal.z();
    rayhit->hit.u = u / plane->ulen();
    rayhit->hit.v = v / plane->vlen();
}

Plane::Plane(const Device& device,
             const Eigen::Vector3f& corner,
             const Eigen::Vector3f& u,
             const Eigen::Vector3f& v)
    : Geometry(device, RTC_GEOMETRY_TYPE_USER), _corner(corner), _u(u), _v(v),
      _n(u.cross(v).normalized()), _ulen(u.norm()), _vlen(v.norm())
{
    rtcSetGeometryUserPrimitiveCount(this->_raw, 1);
    rtcSetGeometryUserData(this->_raw, this);
    rtcSetGeometryBoundsFunction(this->_raw, _plane_bounds, nullptr);
    rtcSetGeometryIntersectFunction(this->_raw, _plane_intersect);
    rtcCommitGeometry(this->_raw);
}

} // namespace yart
