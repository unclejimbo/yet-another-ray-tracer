#pragma once

#include <embree3/rtcore.h>
#include <Eigen/Dense>
#include <vector>

namespace yart
{

class Device;
class Geometry;
class Material;
class Camera;

class Scene
{

public:
    explicit Scene(const Device& device);
    ~Scene();
    void add(Geometry& geom, Material& mat);
    void render(const Camera& camera,
                unsigned char* pixels,
                unsigned width,
                unsigned height,
                unsigned samples = 1,
                bool interleaved = false,
                unsigned depth = 10);

private:
    Eigen::Array3f _path_tracing(RTCIntersectContext& context,
                                 RTCRayHit& rayhit,
                                 unsigned depth);

private:
    RTCScene _raw;
    bool _need_commit;
    std::vector<Material*> _mats;
};

} // namespace yart
