#pragma once

#include "common.h"
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
                    unsigned width,
                    unsigned height,
                    unsigned samples = 1,
                    bool interleaved = false,
                    unsigned depth = 10);

private:
    unsigned _path_tracing(RTCIntersectContext& context,
                           RTCRayHit& rayhit,
                           Eigen::Array3f& irradiance,
                           unsigned depth);

private:
    RTCScene _raw;
    bool _need_commit;
    std::vector<Material*> _mats;
};

} // namespace yart
