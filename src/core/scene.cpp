#include <yart/core/scene.h>
#include <yart/core/device.h>
#include <yart/core/geometry.h>
#include <yart/core/light.h>
#include <yart/core/material.h>
#include <yart/light/area_light.h>

namespace yart
{

Scene::Scene(const Device& device)
{
    _raw = rtcNewScene(device.raw());
    _bg.setZero();
}

Scene::~Scene()
{
    rtcReleaseScene(_raw);
}

void Scene::add(Geometry& geom, Material& mat)
{
    rtcAttachGeometry(_raw, geom.raw());
    _geoms.push_back(&geom);
    _mats.push_back(&mat);
    // TODO: handle DiffuseLight material
}

void Scene::add_light(Light& light)
{
    _lights.push_back(&light);
    // TODO: handle AreaLight geometry
}

void Scene::commit()
{
    rtcCommitScene(_raw);
}

} // namespace yart
