#include <yart/geometry/instance.h>
#include <yart/core/scene.h>

namespace yart
{

Instance::Instance(const Device& device, const Scene& scene)
    : Geometry(device, RTC_GEOMETRY_TYPE_INSTANCE), _scene(&scene)
{
    _trans.setIdentity();
    rtcSetGeometryInstancedScene(this->_raw, scene.raw());
    rtcSetGeometryTimeStepCount(this->_raw, 1);
    rtcSetGeometryUserPrimitiveCount(this->_raw, 1);
    _update();
}

LocalGeometry Instance::sample() const
{
    // TODO:
    return LocalGeometry();
}

void Instance::translate(const Eigen::Vector3f& vec)
{
    _trans = Eigen::Translation3f(vec) * _trans;
    _update();
}

void Instance::rotate(float angle, const Eigen::Vector3f& axis)
{
    _trans = Eigen::AngleAxisf(angle, axis) * _trans;
    _update();
}

void Instance::scale(const Eigen::Vector3f& vec)
{
    _trans = Eigen::AlignedScaling3f(vec) * _trans;
    _update();
}

void Instance::transform(const Eigen::Matrix4f& mat)
{
    _trans = Eigen::Affine3f(mat);
    _update();
}

Eigen::Matrix4f Instance::matrix() const
{
    return _trans.matrix();
}

Eigen::Affine3f Instance::transform() const
{
    return _trans;
}

void Instance::_update()
{
    rtcSetGeometryTransform(
        this->_raw, 0, RTC_FORMAT_FLOAT4X4_COLUMN_MAJOR, _trans.data());
    rtcCommitGeometry(this->_raw);
}

} // namespace yart
