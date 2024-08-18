#include "fff.h"

#include "switches.h"

#ifdef FFF_MOCK_IMPL
    #define VOID_FUNC FAKE_VOID_FUNC
    #define VALUE_FUNC FAKE_VALUE_FUNC
#else
    #define VOID_FUNC DECLARE_FAKE_VOID_FUNC
    #define VALUE_FUNC DECLARE_FAKE_VALUE_FUNC
#endif

#define FFF_USTDLIB_FAKES_LIST(FUNC)    \
    FUNC(usnprintf)


VALUE_FUNC(int, usnprintf, char *, size_t, const char *)