#include "fff.h"
#include "temp_manager.h"

#ifdef FFF_MOCK_IMPL
    #define VOID_FUNC FAKE_VOID_FUNC
    #define VALUE_FUNC FAKE_VALUE_FUNC
#else
    #define VOID_FUNC DECLARE_FAKE_VOID_FUNC
    #define VALUE_FUNC DECLARE_FAKE_VALUE_FUNC
#endif

#define FFF_TEMP_MANAGER_FAKES_LIST(FUNC)  \
    FUNC(tempGetTemp)                 

VALUE_FUNC(float, tempGetTemp);