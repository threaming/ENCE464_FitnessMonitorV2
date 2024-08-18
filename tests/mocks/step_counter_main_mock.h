#include "fff.h"
#include "step_counter_main.h"

#ifdef FFF_MOCK_IMPL
    #define VOID_FUNC FAKE_VOID_FUNC
    #define VALUE_FUNC FAKE_VALUE_FUNC
#else
    #define VOID_FUNC DECLARE_FAKE_VOID_FUNC
    #define VALUE_FUNC DECLARE_FAKE_VALUE_FUNC
#endif

#define FFF_STEP_COUNTER_MAIN_FAKES_LIST(FUNC)         \
    FUNC(flashMessage)   

VOID_FUNC(flashMessage, char*, deviceStateInfo_t*);
