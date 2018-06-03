#include <yart/core/device.h>
#include <string>
#include <stdexcept>

namespace yart
{

Device::Device()
{
    _raw = rtcNewDevice(nullptr);
    if (!_raw) {
        auto err = rtcGetDeviceError(_raw);
        std::string err_str("Embree device creation error: ");
        err_str.append(std::to_string(err));
        throw std::runtime_error(err_str);
    }
}

Device::~Device()
{
    rtcReleaseDevice(_raw);
}

} // namespace yart
