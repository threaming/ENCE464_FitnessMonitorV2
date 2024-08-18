#include "fff.h"
#include <stdbool.h>
#include <stdint.h>
#include "FreeRTOSConfig.h"

#ifdef FFF_MOCK_IMPL
    #define VOID_FUNC FAKE_VOID_FUNC
    #define VALUE_FUNC FAKE_VALUE_FUNC
#else
    #define VOID_FUNC DECLARE_FAKE_VOID_FUNC
    #define VALUE_FUNC DECLARE_FAKE_VALUE_FUNC
#endif

#define FFF_CONFIG_FAKES_LIST(FUNC)  \
    FUNC(vAssertCalled)                 

VOID_FUNC(vAssertCalled, const char *, unsigned long)