#pragma once

#ifdef WIN32
    #ifndef rt_lib
        #define rt_lib __declspec(dllexport)
    #endif
#else
    #define rt_lib
#endif