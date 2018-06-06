#pragma once

#include "../core/common.h"
#include "../core/camera.h"

namespace yart
{

/** A persective camera.
 *
 *  The range of visible frustum of a perspective camera is determined
 *  by the field of view and aspect ratio.
 */
class YART_API PerspectiveCamera : public Camera
{
public:
    /** Create a PerspectiveCamera using default parameters.*/
    PerspectiveCamera();

    /** Create a PerspectiveCamera.
     *
     *  In addition to camera position and orientation, a perspective camera
     *  uses field of view and apsect ratio to determine the extent of the
     *  film plane.
     *
     *  @param position Position.
     *  @param focus Focus.
     *  @param up Rough up direction.
     *  @param vfov Vertical field of view in degrees.
     *  @param aspect Aspect ratio.
     */
    PerspectiveCamera(const Eigen::Vector3f& position,
                      const Eigen::Vector3f& focus = Eigen::Vector3f::Zero(),
                      const Eigen::Vector3f& up = Eigen::Vector3f(0.0f,
                                                                  1.0f,
                                                                  0.0f),
                      float aperture = 2.0f,
                      float vfov = 90.0f,
                      float aspect = 1.0f);

    PerspectiveCamera(const Eigen::Vector3f& position,
                      const Eigen::Vector3f& focus = Eigen::Vector3f::Zero(),
                      const Eigen::Vector3f& up = Eigen::Vector3f(0.0f,
                                                                  1.0f,
                                                                  0.0f),
                      float aperture = 2.0f,
                      float vfov = 90.0f,
                      unsigned width = 256,
                      unsigned height = 256);

    /** Set aspect ratio.*/
    void set_aspect(unsigned width, unsigned height);

    /** Set vertical fov in degrees.*/
    void set_fov(float vfov);

    /** Generate an embree rayhit structure.
     *
     *  The ray's origin be at the camera position and points to the pixel
     *  (s, t) on the film plane.
     */
    RTCRayHit gen_ray(
        float s,
        float t,
        float near = 0.0f,
        float far = std::numeric_limits<float>::max()) const override;
};

} // namespace yart
