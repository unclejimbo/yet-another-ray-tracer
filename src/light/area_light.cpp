#include <yart/light/area_light.h>
#include <yart/core/geometry.h>
#include <yart/material/diffuse_light.h>
#include "../util/rayhit.h"

namespace yart
{

AreaLight::AreaLight(const Geometry& geom, const DiffuseLight& mat)
    : _geom(&geom), _mat(&mat)
{}

bool AreaLight::cast_shadow_ray(const Eigen::Vector3f& target,
                                RTCRay& shadow_ray,
                                Eigen::Array3f& le) const
{
    auto sample = _geom->sample();
    Eigen::Vector3f raydir = target - sample.position;
    if (raydir.dot(sample.normal) > 0.0f) {
        shadow_ray = make_ray(sample.position, raydir, 0.0f, 1.0f - 1e-4f);
        le = _mat->eval(sample.u, sample.v);
        return true;
    }
    else {
        return false;
    }
}

} // namespace yart
