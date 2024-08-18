#include "fff.h"
#include "device_state.h"

#ifdef FFF_MOCK_IMPL
    #define VOID_FUNC FAKE_VOID_FUNC
    #define VALUE_FUNC FAKE_VALUE_FUNC
#else
    #define VOID_FUNC DECLARE_FAKE_VOID_FUNC
    #define VALUE_FUNC DECLARE_FAKE_VALUE_FUNC
#endif

#define FFF_DEVICE_STATE_FAKES_LIST(FUNC)         \
    FUNC(get_modifiable_device_state)   

VALUE_FUNC(deviceStateInfo_t*, get_modifiable_device_state);
