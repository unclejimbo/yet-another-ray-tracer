#pragma once

#include <embree3/rtcore.h>
#include <Eigen/Dense>
#include <limits>

namespace yart
{

/** The film plane.
 *
 */
struct Film
{
    float width;
    float height;
};

/** A simple positionable camera model.
 *
 *  This class uses the right-handed coordinate system.
 */
class Camera
{
public:
    /** Create a Camera with default paramters.*/
    Camera() = default;

    /** Create a Camera.
     *
     *  Position the camera using position, focus and up.
     *
     *  @param position Position.
     *  @param focus Focus.
     *  @param up Rough up direction.
     */
    Camera(const Eigen::Vector3f& position,
           const Eigen::Vector3f& focus = Eigen::Vector3f::Zero(),
           const Eigen::Vector3f& up = Eigen::Vector3f(0.0f, 1.0f, 0.0f),
           float aperture = 2.0f);

    virtual ~Camera() = default;

    /** Position the camera according to the parameteres.*/
    void lookat(const Eigen::Vector3f& position,
                const Eigen::Vector3f& focus,
                const Eigen::Vector3f& up);

    void zoom(float focus_len);

    /** Generate an embree rayhit structure.
     *
     *  Generate a ray for a pixel (s, t) on the film plane.
     *  The parameter of the ray ranges in [near, far). The returned RTCRayHit
     *  structure also has its hit.geomID filed set to RTC_INVALID_GEOMETRY_ID.
     *
     *  @param s The u coordinate on the film plane, ranges in [0, 1).
     *  @param t The v coordinate on the film plane, ranges in [0, 1).
     *  @param near The value of ray parameter for the near end point.
     *  @param far The value of ray parameter for the far end point.
     */
    virtual RTCRayHit gen_ray(
        float s,
        float t,
        float near = 0.0f,
        float far = std::numeric_limits<float>::max()) const = 0;

protected:
    Eigen::Vector3f _pos{ 0.0f, 0.0f, 0.0f };
    Eigen::Vector3f _u{ 1.0f, 0.0f, 0.0f };
    Eigen::Vector3f _v{ 0.0f, 1.0f, 0.0f };
    Eigen::Vector3f _dir{ 0.0f, 0.0f, 1.0f };
    float _focus = 1.0f;
    float _lens_radius = 1.0f;
    Film _film;
};

} // namespace yart
