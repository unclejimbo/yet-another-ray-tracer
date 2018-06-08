#pragma once

#include <yart/core/material.h>

namespace yart
{

class YART_API Lambertian : public Material
{
public:
    explicit Lambertian(const Eigen::Array3f& albedo) : albedo(albedo) {}
    Lambertian(float r, float g, float b) : albedo(Eigen::Array3f(r, g, b)) {}
    bool scatter(const Eigen::Vector3f& rayin,
                 const Eigen::Vector3f& hitpt,
                 const Eigen::Vector3f& normal,
                 Eigen::Vector3f& rayout,
                 Eigen::Array3f& attenuation) const override;

public:
    Eigen::Array3f albedo;
};

} // namespace yart
