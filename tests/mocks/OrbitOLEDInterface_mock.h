#include "fff.h"

#include "OrbitOLED/OrbitOLEDInterface.h"

#ifdef FFF_MOCK_IMPL
    #define VOID_FUNC FAKE_VOID_FUNC
    #define VALUE_FUNC FAKE_VALUE_FUNC
#else
    #define VOID_FUNC DECLARE_FAKE_VOID_FUNC
    #define VALUE_FUNC DECLARE_FAKE_VALUE_FUNC
#endif

#define FFF_ORBITOLED_FAKES_LIST(FUNC)         \
    FUNC(OLEDStringDraw)            \
    FUNC(OLEDInitialise)   

VOID_FUNC(OLEDStringDraw, char *, uint32_t, uint32_t);
VOID_FUNC(OLEDInitialise);

