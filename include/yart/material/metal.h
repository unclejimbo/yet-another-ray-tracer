#pragma once

#include "../core/material.h"

namespace yart
{

class Metal : public Material
{
public:
    Metal(const Eigen::Array3f& albedo, float roughness);
    Metal(float r, float g, float b, float roughness);
    bool scatter(const Eigen::Vector3f& rayin,
                 const Eigen::Vector3f& hitpt,
                 const Eigen::Vector3f& normal,
                 Eigen::Vector3f& rayout,
                 Eigen::Array3f& attenuation) const override;

public:
    Eigen::Array3f albedo;
    float roughness;
};

} // namespace yart
