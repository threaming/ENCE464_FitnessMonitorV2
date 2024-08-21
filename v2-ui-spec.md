# Fitness Monitor v2.0 User Interface Specification

1. The program should respond reliably and in real time to button pushes and a scroll wheel.

   - The scroll wheel input will be provided by the potentiometer on the Orbit board, which produces an analogue signal on pin AIN0.

2. Pushing the RESET button on the TIVA board should restart the program and return any stored step, distance, and goal values to their defaults

   2.1. The default step and distance values are zero

   2.2. The default goal value is 1,000 steps

3. At startup the OLED board should display:

   3.1. The fitness monitor current spec that is loaded

   3.2. The number of steps counted since the last reset or power cycle (initialised to zero)

   3.3. The elapsed time from when the fitness monitor detects its first step (initialised to zero)

   3.4. The current temperature in deg C


There are 3 main screens:
Screen #1 (Fitness monitor main screen - version, steps, time, temp):
   1. The fitness monitor current spec that is loaded

   2. The number of steps counted since the last reset or power cycle (initialised to zero)

   3. The elapsed time from when the fitness monitor detects its first step (initialised to zero)

   4. The current temperature in deg C

Screen #2 (Set goal screen - new goal, current goal):
   1. A new prospective step goal adjusted by the scroll wheel (potentiometer)

   2. The current step goal

Screen #3 (Distance and speed screen - distance, time, speed, walking/running)
   1. The distance travelled based on the number of steps travelled

   2. The elapsed time from when the fitness monitor detects its first step

   3. Average speed since first step

   4. Information about whether you have been walking or running (based on average speed <10km/h = Running)

   



4. LEFT button behaviour:

   4.1. Pushing the LEFT button on the Orbit board should change the display to the total distance travelled since last reset.
   
   4.2. Pushing the LEFT button again should take the user to a Set Goal state, where the target number of steps can be set.

   4.3. Pushing the LEFT button again should display the number of steps counted since the last reset or power cycle.

5. RIGHT button behaviour. Pushing the RIGHT button should achieve the same change in state as pushing the LEFT button, but in the opposite direction:

   5.1. Pushing the RIGHT button when the number of steps is displayed should take the user to a Set Goal state

   5.2. Pushing the RIGHT button again should show the total distance travelled

   5.3. Pushing the RIGHT button a third time should show the total number of steps since last reset.

6. Pushing the UP button on the TIVA board should toggle the units used by the fitness monitor.

   6.1. For the total number of steps, the units are either the raw number of steps or the number of steps as a percentage of the step goal.

   6.2. For the total distance travelled, the units are either the total distance in kilometres or the total distance in miles.

   6.3. In all cases, numbers shown on the OLED display should be accompanied by units.

7. Briefly pushing the DOWN button on the TIVA board when in the Set Goal state should set the number displayed as the new goal and change the state to show steps counted.

8. When not in the Set Goal state a long press on the DOWN button should reset the number of steps counted and distance travelled to zero.

9. The UI should provide feedback to the user that acknowledges and diﬀerentiates between long and short pushes on the DOWN button.

10. Setting the goal value

    10.1. Rotating the potentiometer on the Orbit board clockwise when in the Set Goal state should increase the number displayed.

    10.2. Rotating the potentiometer on the Orbit board anti-clockwise when in the Set Goal state should decrease the displayed value.

    10.3. The Goal value should increment in units of 100 steps. Note that the goal value should only be overwritten with the displayed value when the DOWN button is briefly pushed.

11. Setting SW1 to the UP position should put the fitness monitor in a test mode, where the functionality of the GUI can be verified. In the test mode:

    11.1. Each push of the UP button should increment the step count by 100 and the distance by 0.09 km.

    11.2. Each push of the DOWN button should decrement the step count by 500 and the distance by 0.45 km.

    11.3. Other functions of the UP and DOWN buttons, namely toggling units, setting goals and resetting counts, should be disabled while SW1 is UP.

    11.4. The functionality of the LEFT and RIGHT buttons should not be aﬀected by SW1.

    11.5. Setting SW1 to DOWN should restore the normal functionality of the UP and DOWN buttons.

