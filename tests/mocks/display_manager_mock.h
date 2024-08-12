#include "fff.h"
#include <stdbool.h>
#include "../../libs/OrbitOLED/OrbitOLEDInterface.h"
#include "display_manager.h"

#ifdef FFF_MOCK_IMPL
    #define VOID_FUNC FAKE_VOID_FUNC
    #define VALUE_FUNC FAKE_VALUE_FUNC
#else
    #define VOID_FUNC DECLARE_FAKE_VOID_FUNC
    #define VALUE_FUNC DECLARE_FAKE_VALUE_FUNC
#endif

#define FFF_DISPLAY_MANAGER_FAKES_LIST(FUNC) \
    FUNC(OLEDInitialise)                     \
    FUNC(OLEDStringDraw)                     \
    FUNC(displayLine)                        \
    FUNC(displayValue)                       \
    FUNC(displayTime)

VOID_FUNC(OLEDInitialise);
VOID_FUNC(OLEDStringDraw, const char *, uint32_t, uint32_t);
VOID_FUNC(displayLine, char*, uint8_t, textAlignment_t);
VOID_FUNC(displayValue, char*, char*, int32_t, uint8_t, textAlignment_t, bool);
VOID_FUNC(displayTime, char*, uint16_t, uint8_t, textAlignment_t);
