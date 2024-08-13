#include "fff.h"
#include <stdint.h>
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
    FUNC(OLEDStringDraw)                         


VOID_FUNC(OLEDInitialise);
VOID_FUNC(OLEDStringDraw, const char *, uint32_t, uint32_t);


// Override the OLEDStringDraw mock to store a deep copy of the string as this const char * value kept getting lost --> MAY NEED TO FREE MEMORY IN TEARDOWN later on
// void OLEDStringDraw(const char *str, uint32_t column, uint32_t row) {
//     OLEDStringDraw_fake.arg0_val = strdup(str);
//     OLEDStringDraw_fake.arg1_val = column;
//     OLEDStringDraw_fake.arg2_val = row;
// }