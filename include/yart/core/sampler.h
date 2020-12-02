#pragma once

#include <yart/core/common.h>
#include <Eigen/Core>
#include <random>

namespace yart
{

class YART_API Sampler
{
public:
    Sampler();
    float uniform_1d(float* pdf = nullptr);
    Eigen::Vector2f uniform_2d(float* pdf = nullptr);
    Eigen::Vector2f uniform_in_disk(float* pdf = nullptr);
    Eigen::Vector3f uniform_in_sphere(float* pdf = nullptr);
    Eigen::Vector3f uniform_on_sphere(float* pdf = nullptr);
    Eigen::Vector3f uniform_on_hemisphere(float* pdf = nullptr);
    Eigen::Vector3f cosine_weighted_on_hemisphere(float* pdf = nullptr);

private:
    std::random_device _rd;
    std::minstd_rand _rng;
    std::uniform_real_distribution<float> _ud;
};

} // namespace yart
