#include <yart/camera/perspective.h>
#include "../util/assert.h"
#include "../util/random.h"
#include "../util/rayhit.h"

namespace yart
{

PerspectiveCamera::PerspectiveCamera() : Camera()
{
    this->_film.height = 2.0f * std::tan(90.0f * 0.5f);
    this->_film.width = _film.height;
}

PerspectiveCamera::PerspectiveCamera(const Eigen::Vector3f& position,
                                     const Eigen::Vector3f& focus,
                                     const Eigen::Vector3f& up,
                                     float aperture,
                                     float vfov,
                                     float aspect)
    : Camera(position, focus, up, aperture)
{
    auto fov = vfov * M_PI / 180.0f;
    this->_film.height = 2.0f * std::tan(fov * 0.5f);
    this->_film.width = aspect * this->_film.height;
}

PerspectiveCamera::PerspectiveCamera(const Eigen::Vector3f& position,
                                     const Eigen::Vector3f& focus,
                                     const Eigen::Vector3f& up,
                                     float aperture,
                                     float vfov,
                                     unsigned width,
                                     unsigned height)
    : Camera(position, focus, up, aperture)
{
    auto fov = vfov * M_PI / 180.0f;
    auto aspect = (width + 0.0f) / height;
    this->_film.height = 2.0f * std::tan(fov * 0.5f);
    this->_film.width = aspect * this->_film.height;
}

void PerspectiveCamera::set_aspect(unsigned width, unsigned height)
{
    auto aspect = static_cast<float>(width) / height;
    this->_film.width = aspect * this->_film.height;
}

void PerspectiveCamera::set_fov(float vfov)
{
    auto fov = vfov * M_PI / 180.0f;
    auto aspect = this->_film.width / this->_film.height;
    this->_film.height = 2.0f * std::tan(fov * 0.5f);
    this->_film.width = aspect * this->_film.height;
}

RTCRayHit PerspectiveCamera::gen_ray(float s,
                                     float t,
                                     float near,
                                     float far) const
{
    auto rd = _lens_radius * random_in_unit_disk();
    auto rayorg = this->_pos + rd(0) * this->_u + rd(1) * this->_v;
    Eigen::Vector3f raydir =
        -this->_dir * this->_focus +
        (s - 0.5f) * this->_film.width * this->_u * this->_focus +
        (t - 0.5f) * this->_film.height * this->_v * this->_focus;
    YASSERT(raydir.dot(this->_dir) < 0.0f);

    auto rayhit = make_rayhit(rayorg, raydir, near, far);
    return rayhit;
}

} // namespace yart
