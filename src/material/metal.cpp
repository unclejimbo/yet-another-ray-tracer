#include <yart/material/metal.h>
#include <algorithm>
#include "../util/rayhit.h"
#include "../util/random.h"

namespace yart
{

Metal::Metal(const Eigen::Array3f& albedo, float roughness) : albedo(albedo)
{
    this->roughness = std::clamp(roughness, 0.0f, 1.0f);
}

Metal::Metal(float r, float g, float b, float roughness) : albedo(r, g, b)
{
    this->roughness = std::clamp(roughness, 0.0f, 1.0f);
}

bool Metal::scatter(const Eigen::Vector3f& rayin,
                    const Eigen::Vector3f& hitpt,
                    const Eigen::Vector3f& normal,
                    Eigen::Vector3f& rayout,
                    Eigen::Array3f& attenuation) const

{
    rayout = reflect(rayin, normal.normalized());
    rayout = rayout + roughness * random_in_unit_sphere();
    attenuation = albedo;
    return rayout.dot(normal) > 0.0f;
}

} // namespace yart
