#pragma once

#include <yart/core/geometry.h>
#include <embree3/rtcore.h>
#include <memory>

namespace yart
{

class YART_API UserGeometryData
{
public:
    ~UserGeometryData() = default;
    virtual void bounds(const RTCBoundsFunctionArguments* args) = 0;
    virtual void intersect(const RTCIntersectFunctionNArguments* args) = 0;
    virtual void occluded(const RTCOccludedFunctionNArguments* args) = 0;
};

class YART_API UserGeometry : public Geometry
{
public:
    UserGeometry(const Device& device, int prim_count = 1);
    virtual ~UserGeometry() = default;
    void bounds(const RTCBoundsFunctionArguments* args) { _data->bounds(args); }
    void intersect(const RTCIntersectFunctionNArguments* args)
    {
        _data->intersect(args);
    }
    void occluded(const RTCOccludedFunctionNArguments* args)
    {
        _data->occluded(args);
    }

protected:
    std::unique_ptr<UserGeometryData> _data;
};

} // namespace yart
