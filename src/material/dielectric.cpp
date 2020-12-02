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

Eigen::Vector3f Dielectric::sample(const RTCRayHit& rayhit, float& pdf) const
{
    pdf = 1.0f;

    Eigen::Vector3f wi = Eigen::Vector3f::Zero();
    auto wo = get_raydir(rayhit);
    auto normal = get_hitnormal(rayhit);

    Eigen::Vector3f outward_normal;
    float ni_over_nt, cosine, reflect_prob;
    if (wo.dot(normal) > 0.0f) { // solid to air
        outward_normal = -normal;
        ni_over_nt = rindex;
        cosine = rindex * wo.normalized().dot(normal);
    }
    else { // air to solid
        outward_normal = normal;
        ni_over_nt = 1.0f / rindex;
        cosine = -wo.normalized().dot(normal);
    }

    auto refracted = refract(wo, outward_normal.normalized(), ni_over_nt);
    if (refracted.has_value()) {
        reflect_prob = _schlick_fresnel(cosine, rindex);
        if (Material::_sampler.uniform_1d() < reflect_prob) { // fresnel
            wi = reflect(wo, normal.normalized());
        }
        else { // transmission
            wi = *refracted;
        }
    }
    else { // total reflection
        wi = reflect(wo, normal.normalized());
    }
    return wi;
}

Eigen::Array3f Dielectric::eval(const RTCRayHit& rayhit,
                                const Eigen::Vector3f& wi) const
{
    return Eigen::Array3f::Ones();
}

} // namespace yart
