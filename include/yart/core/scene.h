#pragma once

#include <yart/core/common.h>
#include <embree3/rtcore.h>
#include <Eigen/Core>
#include <vector>

namespace yart
{

class Camera;
class Device;
class Geometry;
class Light;
class Material;

class YART_API Scene
{
public:
    explicit Scene(const Device& device);
    ~Scene();
    RTCScene raw() const { return _raw; }
    void add(Geometry& geom, Material& mat);
    void add_light(Light& light);
    void commit();
    const Camera* camera() const { return _cam; }
    void set_camera(Camera& cam) { _cam = &cam; }
    Eigen::Vector3f background() const { return _bg; }
    void set_background(const Eigen::Vector3f& bg) { _bg = bg; }
    const std::vector<Geometry*>& geometries() const { return _geoms; }
    const std::vector<Material*>& materials() const { return _mats; }
    const std::vector<Light*>& lights() const { return _lights; }

private:
    RTCScene _raw;
    Camera* _cam;
    std::vector<Geometry*> _geoms;
    std::vector<Material*> _mats;
    std::vector<Light*> _lights;
    Eigen::Vector3f _bg;
};

} // namespace yart
