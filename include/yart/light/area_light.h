#pragma once

#include <yart/core/light.h>

namespace yart
{

class Geometry;
class DiffuseLight;

class YART_API AreaLight : public Light
{
public:
    AreaLight(const Geometry& geom, const DiffuseLight& mat);
    bool cast_shadow_ray(const Eigen::Vector3f& target,
                         RTCRay& shadow_ray,
                         Eigen::Array3f& le) const override;

private:
    const Geometry* _geom;
    const DiffuseLight* _mat;
};

} // namespace yart
