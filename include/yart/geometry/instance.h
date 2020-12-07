#pragma once

#include <yart/core/geometry.h>
#include <Eigen/Core>
#include <Eigen/Geometry>

namespace yart
{

class Scene;

class YART_API Instance : public Geometry
{
public:
    Instance(const Device& device, const Scene& scene);
    LocalGeometry sample() const override;
    void translate(const Eigen::Vector3f& vec);
    void rotate(float angle, const Eigen::Vector3f& axis);
    void scale(const Eigen::Vector3f& vec);
    void transform(const Eigen::Matrix4f& mat);
    Eigen::Matrix4f matrix() const;
    Eigen::Affine3f transform() const;

private:
    void _update();

private:
    const Scene* _scene = nullptr;
    Eigen::Affine3f _trans;
};

} // namespace yart
