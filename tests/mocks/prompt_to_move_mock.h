#include "fff.h"
#include <stdint.h>
#include "prompt_to_move.h"

#ifdef FFF_MOCK_IMPL
    #define VOID_FUNC FAKE_VOID_FUNC
    #define VALUE_FUNC FAKE_VALUE_FUNC
#else
    #define VOID_FUNC DECLARE_FAKE_VOID_FUNC
    #define VALUE_FUNC DECLARE_FAKE_VALUE_FUNC
#endif

#define FFF_PROMPT_TO_MOVE_FAKES_LIST(FUNC)    \
    FUNC(act_on_prompt_to_move)              

VOID_FUNC(act_on_prompt_to_move);

