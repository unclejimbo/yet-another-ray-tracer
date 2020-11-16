#pragma once

#include <Eigen/Core>
#include <random>

namespace yart
{

inline Eigen::Vector3f random_in_unit_sphere()
{
    std::random_device rd;
    std::minstd_rand rd_gen(rd());
    std::uniform_real_distribution<> rd_number(-1.0f, 1.0f);
    Eigen::Vector3f p;
    do {
        p = Eigen::Vector3f(
            rd_number(rd_gen), rd_number(rd_gen), rd_number(rd_gen));
    } while (p.dot(p) >= 1.0f);
    return p;
}

inline Eigen::Vector3f random_in_unit_disk()
{
    std::random_device rd;
    std::minstd_rand rd_gen(rd());
    std::uniform_real_distribution<> rd_number(-1.0f, 1.0f);
    Eigen::Vector3f p;
    do {
        p = Eigen::Vector3f(rd_number(rd_gen), rd_number(rd_gen), 0.0f);
    } while (p.dot(p) >= 1.0f);
    return p;
}

} // namespace yart
