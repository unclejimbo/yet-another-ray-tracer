#define _USE_MATH_DEFINES
#include <yart/geometry/sphere.h>
#include <cmath>
#include "../util/macro.h"
#include "../util/rayhit.h"

namespace yart
{

SphereData::SphereData(float radius, const Eigen::Vector3f& center)
    : _radius(radius), _center(center)
{}

void SphereData::bounds(const RTCBoundsFunctionArguments* args)
{
    auto bounds_o = args->bounds_o;
    bounds_o->lower_x = _center(0) - _radius;
    bounds_o->lower_y = _center(1) - _radius;
    bounds_o->lower_z = _center(2) - _radius;
    bounds_o->upper_x = _center(0) + _radius;
    bounds_o->upper_y = _center(1) + _radius;
    bounds_o->upper_z = _center(2) + _radius;
}

void SphereData::intersect(const RTCIntersectFunctionNArguments* args)
{
    YASSERT(args->N == 1);

    auto rayhit = reinterpret_cast<RTCRayHit*>(args->rayhit);
    auto rayorg = get_rayorg(*rayhit);
    auto raydir = get_raydir(*rayhit);

    auto oc = rayorg - _center;

    auto a = raydir.dot(raydir);
    auto b = 2.0f * oc.dot(raydir);
    auto c = oc.dot(oc) - _radius * _radius;
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
        Eigen::Vector3f ng = (hitpt - _center).normalized();
        rayhit->hit.Ng_x = ng(0);
        rayhit->hit.Ng_y = ng(1);
        rayhit->hit.Ng_z = ng(2);
        auto phi = std::atan2(ng(0), ng(2));
        auto theta = std::asin(ng(1));
        rayhit->hit.u = 1.0f - (phi + M_PI) / (M_PI * 2.0f);
        rayhit->hit.v = (theta + M_PI_2) / M_PI;
    }
}

void SphereData::occluded(const RTCOccludedFunctionNArguments* args)
{
    YASSERT(args->N == 1);

    auto ray = reinterpret_cast<RTCRay*>(args->ray);
    auto rayorg = get_rayorg(*ray);
    auto raydir = get_raydir(*ray);

    auto oc = rayorg - _center;

    auto a = raydir.dot(raydir);
    auto b = 2.0f * oc.dot(raydir);
    auto c = oc.dot(oc) - _radius * _radius;
    auto d = b * b - 4.0f * a * c;
    if (d < 0.0f) return; // no hit
    auto q = std::sqrt(d);
    auto denom = 0.5f / a;
    auto t0 = denom * (-b - q);
    auto t1 = denom * (-b + q);

    auto old_tfar = ray->tfar;
    if (ray->tnear < t0 && t0 < ray->tfar) { ray->tfar = t0; }
    if (ray->tnear < t1 && t1 < ray->tfar) { ray->tfar = t1; }
}

Sphere::Sphere(const Device& device,
               float radius,
               const Eigen::Vector3f& center)
    : UserGeometry(device)
{
    this->_data = std::make_unique<SphereData>(radius, center);
}

LocalGeometry Sphere::sample() const
{
    // TODO:
    return LocalGeometry();
}

} // namespace yart
