#include <yart/material/dielectric.h>
#include "../util/rayhit.h"

namespace yart
{

bool Dielectric::scatter(const Eigen::Vector3f& rayin,
                         const Eigen::Vector3f& hitpt,
                         const Eigen::Vector3f& normal,
                         Eigen::Vector3f& rayout,
                         Eigen::Array3f& attenuation) const
{
    attenuation = Eigen::Array3f::Ones();
    Eigen::Vector3f outward_normal;
    float ni_over_nt;
    if (rayin.dot(normal) > 0.0f) { // solid to air
        outward_normal = -normal;
        ni_over_nt = rindex;
    }
    else { // air to solid
        outward_normal = normal;
        ni_over_nt = 1.0f / rindex;
    }
    auto refracted = refract(rayin, outward_normal.normalized(), ni_over_nt);
    if (refracted.has_value()) {
        rayout = *refracted;
        return true;
    }
    else {
        rayout = reflect(rayin, normal.normalized());
        return false;
    }
}

} // namespace yart
