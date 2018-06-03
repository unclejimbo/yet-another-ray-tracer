#pragma once

#include <embree3/rtcore.h>

namespace yart
{

class Device
{
    friend class Geometry;
    friend class Scene;

public:
    Device();
    ~Device();

private:
    RTCDevice _raw;
};

} // namespace yart
