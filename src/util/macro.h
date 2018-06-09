#pragma once

#include <iostream>

// Assertions are automatically turned off on release build.
#ifndef NDEBUG
#include <cassert>
#define YASSERT(expr) assert(expr)
#define YASSERT_MSG(expr, msg)                                                 \
    if (!(expr)) {                                                             \
        std::cerr << "yart assertion failed: " << msg << std::endl;            \
        assert(expr);                                                          \
    }
#else // #ifndef NDEBUG
#define YASSERT(expr) void(0);
#define YASSERT_MSG(expr, msg) void(0);
#endif // #ifndef NDEBUG

// Warning is meant to report improper behaviors of the program,
// which may not hinder the program from running.
// #define YART_NO_WARNING before including this header to disable the
// warning messages.
#ifndef YART_NO_WARNING
#define YWARNING(msg)                                                          \
    std::cerr << "yart warning: " << msg << std::endl                          \
              << "(In file " << __FILE__ << ", line " << __LINE__              \
              << ", function " << __func__ << ")" << std::endl;
#else // #ifndef YART_NO_WARNING
#define YWARNING(msg) void(0);
#endif // #ifndef YART_NO_WARNING

#define YUNUSED(expr)                                                          \
    do {                                                                       \
        (void)(expr);                                                          \
    } while (0)
