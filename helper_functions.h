#ifndef helper_functions
#define helper_functions

#ifdef _WIN32
    #include <windows.h>
#else
    #include <unistd.h>
#endif

void sleepcp(int milliseconds);

#endif