#pragma once

#include <yart/core/common.h>
#include <embree3/rtcore.h>

namespace yart
{

class YART_API Device
{
public:
    Device();
    ~Device();
    RTCDevice raw() const { return _raw; }

private:
    RTCDevice _raw;
};

} // namespace yart
