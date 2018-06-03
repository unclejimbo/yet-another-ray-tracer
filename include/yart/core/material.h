#pragma once

#include <embree3/rtcore.h>
#include <Eigen/Dense>

namespace yart
{

class Material
{
public:
    virtual ~Material() = default;
    virtual bool scatter(const Eigen::Vector3f& rayin,
                         const Eigen::Vector3f& hitpt,
                         const Eigen::Vector3f& normal,
                         Eigen::Vector3f& rayout,
                         Eigen::Array3f& attenuation) const = 0;
};

} // namespace yart
