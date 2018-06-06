#pragma once

#include "common.h"
#include <embree3/rtcore.h>

namespace yart
{

class YART_API Device
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
