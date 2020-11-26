#include <yart/core/scene.h>
#include <yart/core/device.h>
#include <yart/core/geometry.h>
#include <yart/core/material.h>

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
}

void Scene::commit()
{
    rtcCommitScene(_raw);
}

} // namespace yart
