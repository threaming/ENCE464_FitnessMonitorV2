#ifndef PROMPT_TO_MOVE_H
#define PROMPT_TO_MOVE_H

// Function to initialize the prompt to move module
void init_prompt_to_move(void);

// External handler function declaration
void act_on_prompt_to_move(void);

// External function to reset the timer if steps have been taken
void reset_timer_if_steps_taken(void);

#endif // PROMPT_TO_MOVE_H