//TODO: Need to include where flashMessage function comes from and CURRENT STEP COUNT
#include "prompt_to_move.h"
#include "hal/timer_hal.h"
#include "display_manager.h"
#include "step_counter_main.h" 
#include "stdio.h"
#include "device_state.h"


static uint32_t previousStepsTaken = 0;

// Handler function to be called when the prompt to move is detected
void act_on_prompt_to_move(void)
{
    deviceStateInfo_t* deviceState = get_modifiable_device_state();

    if (deviceState->stepsTaken == previousStepsTaken)
    {
        if (deviceState->stepsTaken == 0)
        {
            flashMessage("Get Moving!", deviceState, 1);
            // flashMessage("Get Moving!", deviceState, 1);
        } else {
            flashMessage("Keep Moving!", deviceState, 1);
            // flashMessage("Keep Moving!", deviceState, 5);
        }
    }

    previousStepsTaken = deviceState->stepsTaken;

}

void init_prompt_to_move(void)
{
    // Initialize the timer
    timer_hal_init(5);
    // Start the timer task
    timer_hal_start_task();
}

void reset_timer_if_steps_taken(void)
{
    // // Reset the timer if steps have been taken
    // if (stepsChanged> 0) //TODO: Need to change once main has been refactored
    // {
    //     timer_hal_reset(30);
    // }
}

