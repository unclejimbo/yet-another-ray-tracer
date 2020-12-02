#include <yart/material/dielectric.h>
#include <cmath>
#include "../util/rayhit.h"

namespace yart
{

static inline float _schlick_fresnel(float cosine, float rindex)
{
    float r0 = (1.0f - rindex) / (1.0f + rindex);
    r0 = r0 * r0;
    return r0 + (1.0f - r0) * std::pow((1.0f - cosine), 5);
}

bool Dielectric::scatter(const RTCRayHit& rayhit,
                         Eigen::Vector3f& rayout,
                         Eigen::Array3f& attenuation) const
{
    attenuation = Eigen::Array3f::Ones();
    auto rayin = get_raydir(rayhit);
    auto normal = get_hitnormal(rayhit);

    Eigen::Vector3f outward_normal;
    float ni_over_nt, cosine, reflect_prob;
    if (rayin.dot(normal) > 0.0f) { // solid to air
        outward_normal = -normal;
        ni_over_nt = rindex;
        cosine = rindex * rayin.normalized().dot(normal);
    }
    else { // air to solid
        outward_normal = normal;
        ni_over_nt = 1.0f / rindex;
        cosine = -rayin.normalized().dot(normal);
    }

    auto refracted = refract(rayin, outward_normal.normalized(), ni_over_nt);
    if (refracted.has_value()) {
        reflect_prob = _schlick_fresnel(cosine, rindex);
        if (Material::_sampler.uniform_1d() < reflect_prob) { // fresnel
            rayout = reflect(rayin, normal.normalized());
        }
        else { // transmission
            rayout = *refracted;
        }
    }
    else { // total reflection
        rayout = reflect(rayin, normal.normalized());
    }

    return true;
}

} // namespace yart
