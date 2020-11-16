#define _USE_MATH_DEFINES
#include <yart/geometry/sphere.h>
#include <cmath>
#include "../util/macro.h"
#include "../util/rayhit.h"

namespace yart
{

void SphereData::bounds(const RTCBoundsFunctionArguments* args)
{
    auto bounds_o = args->bounds_o;
    auto sphere = reinterpret_cast<SphereData*>(args->geometryUserPtr);
    auto center = sphere->center();
    auto radius = sphere->radius();
    bounds_o->lower_x = center(0) - radius;
    bounds_o->lower_y = center(1) - radius;
    bounds_o->lower_z = center(2) - radius;
    bounds_o->upper_x = center(0) + radius;
    bounds_o->upper_y = center(1) + radius;
    bounds_o->upper_z = center(2) + radius;
}

void SphereData::intersect(const RTCIntersectFunctionNArguments* args)
{
    YASSERT(args->N == 1);

    auto rayhit = reinterpret_cast<RTCRayHit*>(args->rayhit);
    auto rayorg = get_rayorg(*rayhit);
    auto raydir = get_raydir(*rayhit);

    auto sphere = reinterpret_cast<SphereData*>(args->geometryUserPtr);
    auto center = sphere->center();
    auto oc = rayorg - center;
    auto radius = sphere->radius();

    auto a = raydir.dot(raydir);
    auto b = 2.0f * oc.dot(raydir);
    auto c = oc.dot(oc) - radius * radius;
    auto d = b * b - 4.0f * a * c;
    if (d < 0.0f) return; // no hit
    auto q = std::sqrt(d);
    auto denom = 0.5f / a;
    auto t0 = denom * (-b - q);
    auto t1 = denom * (-b + q);

    auto old_tfar = rayhit->ray.tfar;
    if (rayhit->ray.tnear < t0 && t0 < rayhit->ray.tfar) {
        rayhit->ray.tfar = t0;
    }
    if (rayhit->ray.tnear < t1 && t1 < rayhit->ray.tfar) {
        rayhit->ray.tfar = t1;
    }
    if (rayhit->ray.tfar != old_tfar) {
        rayhit->hit.instID[0] = args->context->instID[0];
        rayhit->hit.geomID = args->geomID;
        rayhit->hit.primID = args->primID;
        Eigen::Vector3f hitpt = rayorg + rayhit->ray.tfar * raydir;
        Eigen::Vector3f ng = (hitpt - center).normalized();
        rayhit->hit.Ng_x = ng(0);
        rayhit->hit.Ng_y = ng(1);
        rayhit->hit.Ng_z = ng(2);
        auto phi = std::atan2(ng(0), ng(2));
        auto theta = std::asin(ng(1));
        rayhit->hit.u = 1.0f - (phi + M_PI) / (M_PI * 2.0f);
        rayhit->hit.v = (theta + M_PI_2) / M_PI;
    }
}

SphereData::SphereData(float radius, const Eigen::Vector3f& center)
    : _radius(radius), _center(center)
{}

Sphere::Sphere(const Device& device,
               float radius,
               const Eigen::Vector3f& center)
    : Geometry(device, RTC_GEOMETRY_TYPE_USER), _data(radius, center)
{
    rtcSetGeometryUserPrimitiveCount(this->_raw, 1);
    rtcSetGeometryUserData(this->_raw, &_data);
    rtcSetGeometryBoundsFunction(this->_raw, &SphereData::bounds, nullptr);
    rtcSetGeometryIntersectFunction(this->_raw, &SphereData::intersect);
    rtcCommitGeometry(this->_raw);
}

} // namespace yart
