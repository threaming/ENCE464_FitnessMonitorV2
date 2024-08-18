//TODO: Need to include where flashMessage function comes from and CURRENT STEP COUNT
#include "prompt_to_move.h"
#include "hal/timer_hal.h"
#include "display_manager.h"



// Handler function to be called when the prompt to move is detected
void act_on_prompt_to_move(void)
{
    flashMessage("Move!"); //TODO: Need to change once main has been refactored
}

void init_prompt_to_move(void)
{
    // Initialize the timer
    timer_hal_init(30);
    // Start the timer task
    timer_hal_start_task();
}

void reset_timer_if_steps_taken(void)
{
    // Reset the timer if steps have been taken
    if (stepsChanged> 0) //TODO: Need to change once main has been refactored
    {
        timer_hal_reset(30);
    }
}

