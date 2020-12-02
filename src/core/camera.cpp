#include <yart/core/camera.h>
#include <Eigen/Geometry>

namespace yart
{

Sampler Camera::_sampler;

Camera::Camera(const Eigen::Vector3f& position,
               const Eigen::Vector3f& focus,
               const Eigen::Vector3f& up,
               float aperture)
{
    _pos = position;
    _dir = (position - focus).normalized();
    _u = up.cross(_dir).normalized();
    _v = _dir.cross(_u);
    _focus = (focus - position).norm();
    _lens_radius = aperture * 0.5f;
}

void Camera::lookat(const Eigen::Vector3f& position,
                    const Eigen::Vector3f& focus,
                    const Eigen::Vector3f& up)
{
    _pos = position;
    _dir = (position - focus).normalized();
    _u = up.cross(_dir).normalized();
    _v = _dir.cross(_u);
}

void Camera::zoom(float focus_len)
{
    _focus = focus_len;
}

} // namespace yart
