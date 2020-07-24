#pragma once
#include <assert.h>

#if DEBUG
//#define FW_ASSERT	assert
#define FW_ASSERT(_expression)	\
    do {\
        if (!(_expression)) {\
            __debugbreak();	\
        }\
    } while(false)
#else
// Release
#define FW_ASSERT(_expression)	(void)(_expression)
#endif

#define FW_NOP(_val)	(void)(_val)
