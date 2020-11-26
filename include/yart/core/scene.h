#pragma once

#include <yart/core/common.h>
#include <embree3/rtcore.h>
#include <Eigen/Core>
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
    RTCScene raw() const { return _raw; }
    void add(Geometry& geom, Material& mat);
    void commit();
    const Camera* camera() const { return _cam; }
    void set_camera(Camera& cam) { _cam = &cam; }
    Eigen::Vector3f background() const { return _bg; }
    void set_background(const Eigen::Vector3f& bg) { _bg = bg; }
    const std::vector<Geometry*>& geometries() const { return _geoms; }
    const std::vector<Material*>& materials() const { return _mats; }

private:
    RTCScene _raw;
    Camera* _cam;
    std::vector<Geometry*> _geoms;
    std::vector<Material*> _mats;
    Eigen::Vector3f _bg;
};

} // namespace yart
