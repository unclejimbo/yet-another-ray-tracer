#pragma once

#if defined(_MSC_VER) && !defined(YART_STATIC_LIB)
    #ifdef YART_EXPORTS
        #define YART_API __declspec(dllexport)
    #else
        #define YART_API __declspec(dllimport)
    #endif
#else
        #define YART_API
#endif 
