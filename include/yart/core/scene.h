#pragma once

#include <yart/core/common.h>
#include <embree3/rtcore.h>
#include <Eigen/Dense>
#include <vector>

namespace yart
{

class Device;
class Geometry;
class Material;
class Camera;

class YART_API Scene
{

public:
    explicit Scene(const Device& device);
    ~Scene();
    void add(Geometry& geom, Material& mat);
    unsigned render(const Camera& camera,
                    unsigned char* pixels,
                    int width,
                    int height,
                    int samples = 1,
                    bool interleaved = false,
                    int depth = 10);
    Eigen::Vector3f background() const { return _bg; }
    void set_background(const Eigen::Vector3f& bg) { _bg = bg; }

private:
    unsigned _path_tracing(RTCIntersectContext& context,
                           RTCRayHit& rayhit,
                           Eigen::Array3f& irradiance,
                           unsigned depth);

private:
    RTCScene _raw;
    bool _need_commit;
    std::vector<Material*> _mats;
    Eigen::Vector3f _bg;
};

} // namespace yart
