#include <yart/material/lambertian.h>
#include "../util/random.h"

namespace yart
{

bool Lambertian::scatter(const Eigen::Vector3f& rayin,
                         const Eigen::Vector3f& hitpt,
                         const Eigen::Vector3f& normal,
                         Eigen::Vector3f& rayout,
                         Eigen::Array3f& attenuation) const
{
    rayout = hitpt + normal.normalized() + random_in_unit_sphere();
    rayout -= hitpt;
    attenuation = albedo;
    return true;
}

} // namespace yart
