# Fitness Monitor v2.0 User Interface Specification

1. The program should respond reliably and in real time to button pushes and a scroll wheel.

   - The scroll wheel input will be provided by the potentiometer on the Orbit board, which produces an analogue signal on pin AIN0.


2. There are 3 main screens:

   2.1 Screen #1 (Fitness monitor main screen - version, steps, time, temp):

   - The fitness monitor current spec that is loaded

   - The number of steps counted since the last reset or power cycle (initialised to zero)

   - The elapsed time from when the fitness monitor detects its first step (initialised to zero)

   - The current temperature in deg C

   2.2 Screen #2 (Set goal screen - new goal, current goal):
      
   - A new prospective step goal adjusted by the scroll wheel (potentiometer)

   - The current step goal

   2.3 Screen #3 (Distance and speed screen - distance, time, speed, walking/running)
      
   - The distance travelled based on the number of steps travelled

   - The elapsed time from when the fitness monitor detects its first step

   - Average speed since first step

   - Information about whether you have been walking or running (based on average speed <10km/h = Running)


3. At startup the OLED board should display screen #1 with all information set to their defaults:
   
   3.1 The default step and distance values are zero

   3.2 The default goal value is 1,000 steps


4. Pushing the RESET button on the TIVA board should restart the program and return any stored step, distance, and goal values to their defaults


5. RIGHT button behaviour:

   5.1 Pushing the RIGHT button on the Orbit board should change the display from screen #1 to screen #2.
   
   5.2 Pushing the RIGHT button again should take the user to screen #3.

   5.3 Pushing the RIGHT button again should wrap around and take the user back to screen #1.


6. LEFT button behaviour. Pushing the LEFT button should achieve the same change in state as pushing the RIGHT button, but in the opposite direction:

   6.1 Pushing the LEFT button when the display is on screen #1 should change it to screen #3.

   6.2 Pushing the LEFT button again should take the user to screen #2.

   6.3 Pushing the LEFT button a third time should wrap around and return the user to screen #1.


7. Pushing the UP button on the TIVA board should toggle the units used by the fitness monitor.

   7.1 For the total number of steps, the units are either the raw number of steps or the number of steps as a percentage of the step goal.

   7.2 For the total distance travelled, the units are either the total distance in kilometres or the total distance in miles.

   7.3 In all cases, numbers shown on the OLED display should be accompanied by units.


8. Briefly pushing the DOWN button on the TIVA board when on screen #2 (set goal screen) should set the number displayed as the new goal and change the state to show steps counted.


9. When not on screen #2 (set goal screen) a long press on the DOWN button should reset the number of steps counted and distance travelled to zero.


10. The UI should provide feedback to the user that acknowledges and diﬀerentiates between long and short pushes on the DOWN button.


11. Setting the goal value.

   11.1 Rotating the potentiometer on the Orbit board clockwise when in the Set Goal state should increase the number displayed.

   11.2 Rotating the potentiometer on the Orbit board anti-clockwise when in the Set Goal state should decrease the displayed value.

   11.3 The Goal value should increment in units of 100 steps. Note that the goal value should only be overwritten with the displayed value when the DOWN button is briefly pushed.


12. Setting SW1 to the UP position should put the fitness monitor in a test mode, where the functionality of the GUI can be verified. In the test mode.

   12.1 Each push of the UP button should increment the step count by 100 and the distance by 0.09 km.

   12.2 Each push of the DOWN button should decrement the step count by 500 and the distance by 0.45 km.

   12.3 Other functions of the UP and DOWN buttons, namely toggling units, setting goals and resetting counts, should be disabled while SW1 is UP.

   12.4 The functionality of the LEFT and RIGHT buttons should not be aﬀected by SW1.

   12.5 Setting SW1 to DOWN should restore the normal functionality of the UP and DOWN buttons.

   
13. Summary of extended functionality from fitness monitor v1.0.

   13.1 When the user is on screen #1 the current ambient temperature is shown on the display in deg C.

   13.2 When the user is on screen #3 the device displays whether the user has been running or walking based on their average speed.
      
   - If the user has an average speed >= 10 km/h the screen displays, "Running!".

   - If the user has an average speed < 10 km/h the screen displays, "Walking.".

   13.3 When steps are not detected for more than 60 seconds a prompt to move flashes on the screen. This overrides the current screen, but the device can still detect steps.

   - If the user has not taken any steps since startup the prompt, "Get Moving!" flashes on the screen.

   -  If the user has taken steps since startup but has been stationary for the last 60 seconds the prompt, "Keep Moving!" flashes on the screen.



