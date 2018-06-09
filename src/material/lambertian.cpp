#include <yart/material/lambertian.h>
#include "../util/random.h"
#include "../util/rayhit.h"

namespace yart
{

bool Lambertian::scatter(const RTCRayHit& rayhit,
                         Eigen::Vector3f& rayout,
                         Eigen::Array3f& attenuation) const
{
    auto hitpt = get_hitpt(rayhit);
    auto normal = get_hitnormal(rayhit);
    rayout = hitpt + normal.normalized() + random_in_unit_sphere();
    rayout -= hitpt;
    attenuation = texture->value(rayhit.hit.u, rayhit.hit.v, hitpt);
    return true;
}

} // namespace yart
