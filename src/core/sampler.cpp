#define _USE_MATH_DEFINES
#include <yart/core/sampler.h>
#include <cmath>

namespace yart
{

Sampler::Sampler() : _rng(_rd()), _ud(0.0f, 1.0f) {}

float Sampler::uniform_1d(float* pdf)
{
    if (pdf != nullptr) { *pdf = 1.0f; }
    return _ud(_rng);
}

Eigen::Vector2f Sampler::uniform_2d(float* pdf)
{
    if (pdf != nullptr) { *pdf = 1.0f; }
    return Eigen::Vector2f(_ud(_rng), _ud(_rng));
}

Eigen::Vector2f Sampler::uniform_in_disk(float* pdf)
{
    if (pdf != nullptr) { *pdf = M_1_PI; }
    Eigen::Vector2f p;
    do {
        p << _ud(_rng), _ud(_rng);
    } while (p.dot(p) >= 1.0f);
    return p;
}

Eigen::Vector3f Sampler::uniform_in_sphere(float* pdf)
{
    if (pdf != nullptr) { *pdf = M_PI_4; }
    Eigen::Vector3f p;
    do {
        for (int i = 0; i < 3; ++i) {
            p(i) = _ud(_rng) * 2.0f - 1.0f;
        }
    } while (p.dot(p) >= 1.0f);
    return p;
}

Eigen::Vector3f Sampler::uniform_on_sphere(float* pdf)
{
    if (pdf != nullptr) { *pdf = M_PI_4; }
    float cos_theta = 1.0f - 2.0f * _ud(_rng);
    float sin_theta = std::sqrt(1.0f - cos_theta * cos_theta);
    float phi = _ud(_rng) * M_PI * 2.0f;
    return Eigen::Vector3f(
        sin_theta * std::cos(phi), cos_theta, sin_theta * std::sin(phi));
}

Eigen::Vector3f Sampler::uniform_on_hemisphere(float* pdf)
{
    if (pdf != nullptr) { *pdf = M_PI_2; }
    float cos_theta = 1.0f - _ud(_rng);
    float sin_theta = std::sqrt(1.0f - cos_theta * cos_theta);
    float phi = _ud(_rng) * M_PI * 2.0f;
    return Eigen::Vector3f(
        sin_theta * std::cos(phi), cos_theta, sin_theta * std::sin(phi));
}

Eigen::Vector3f Sampler::cosine_weighted_on_hemisphere(float* pdf)
{
    float cos_theta = std::sqrt(1.0f - _ud(_rng));
    float sin_theta = std::sqrt(1.0f - cos_theta * cos_theta);
    float phi = _ud(_rng) * M_PI * 2.0f;
    if (pdf != nullptr) { *pdf = cos_theta / M_PI; }
    return Eigen::Vector3f(
        sin_theta * std::cos(phi), cos_theta, sin_theta * std::sin(phi));
}

} // namespace yart
