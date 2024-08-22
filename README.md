# ENCE464 - Fitness Monitor
**Group Number:** 12
**Group Members:** Alister Giles, Matt Walker, Andy Ming

## Table of Contents
- [Introduction](#introduction)
- [Analysis of Existing Architecture](#analysis-of-existing-architecture)
- [Summary of software design problem](#summary-of-software-design-problem)
- [Design of New Architecture (v2.0)](#design-of-new-architecture-v20)
- [Design of FreeRTOS tasking](#design-of-freertos-tasking)
- [I<sup>2</sup>C Implementation](#I<sup>2</sup>C-Implementation)
- [Application of static and dynamic analysis](#application-of-static-and-dynamic-analysis)
- [Usefulness of documentation to future developers](#usefulness-of-documentation-to-future-developers)

## Introduction

The purpose of this project was to extend the functionality of an existing *Fitness Monitor V1.0* embedded software project. This project implements a simple step counter based on the provided specifications. This documentation summarises the findings and development of an extended version *Fitness Monitor V2.0* of the project. 
The software version 1.0 was analysed with key areas for improvements highlighted. The improved architecture of V2.0 is also presented and detailed implementations of key components are discussed. 

## Analysis of Existing Architecture

The architectural structure of Fitness Monitor v1.0 is a layered architecture where a single high-level module (`step_counter_main`) coordinates the actions of hardware-specific low level modules. The dependancies of the Fitness Monitor v1.0 are clearly outlined in the provided diagram. The system is divided into several key modules, each responsible for a specific aspect of the overall functionality. The diagram uses a color-coded legend to differentiate between standard libraries, leaf modules or libraries, and components requiring further investigation. Key modules include `step_counter_main`, `display_manager`, `acc_manager`, `ADC_read`, `button_manager`, and several others that interact to form the system.

Each module's structure is depicted using class diagrams, showing the functions and attributes within each class. For example, the `step_counter_main` module includes functions like `initClock()`, `initDisplay()`, and `readCurrentTick()`, etc. This diagram provides a clear overview of how different components interact and the roles they play in the system.

![v1.0 Architecture Diagram](system_diagram_v1.jpg)

The `step_counter_main` module serves as the central hub, interacting with nearly all other modules. For example, it communicates with the `display_manager` to update the display and with the `acc_manager` to process accelerometer data.

`display_manager` depends directly on the `OrbitOLEDInterface` for hardware-specific operations. `acc_manager` interacts directly with a circular buffer (`circBuf`) to manage accelerometer data and uses the `i2c_driver` for communication with the accelerometer chip. `ADC_read` interacts with `step_counter_main` to provide ADC data when required. `button_manager` and `switches` interact to manage user inputs, which are then relayed to the main module. 

### Initial Issues

Fitness monitor v1.0 met the previous desired specifications, however with the intentions of extending the functionality several issues needed to be addressed. The main issues have to do with code maintainability and portability.

In order to improve the code maintainability the circular dependency from the two modules `display_manager` and `button_manager` back to the main module `step_count_main` through the type definition `deviceStateInfo_t` needed to be addressed. This circular dependancy prevents interchangeability and reuse of these modules, and means that in order to improve code changes need to be made in multiple locations.

Another critical flaw is the direct interaction of most modules with hardware-specific libraries, this reduces the system's portability to other platforms. For example, the `acc_manager` and `ADC_read` modules directly interface with the accelerometer and ADC hardware, respectively, without any abstraction layer. This design choice tightly couples the software with the specific hardware. The absence of a hardware abstraction layer (HAL) means that any attempt to migrate the system to a new hardware platform would require extensive rework. This design also complicates testing since hardware-specific dependencies cannot be easily mocked or simulated.

Another maintainability issue is that the core logic of the application is concentrated within a single loop in the step_counter_main module. This design obscures the overall structure and flow of the application, making it difficult to understand and extend. The absence of any testing framework further exacerbates this issue, as it becomes challenging to verify the correctness of individual modules or identify potential breakages during refactoring. 

To address these issues, several changes will be made to the code including:
    - Implementing an RTOS (Real-Time Operating System) to distribute tasks more effectively, improve code organization, maintainability, and scalability.
    - The Unit test framework (Unity) will be used for both maintaining/refactoring code, verifying correctness, and when using test driven design.
    - Code will be refactored to incorporate SOLID principles in order to keep the code clean.

### Summary of software design problem
Specifies the v2.0 design (Superficially/Sufficiently/Comprehensively)

After extensive investigation into the *Fitness Monitor V1.0* software design, a few key sections demanded immediate attention.
- The application needs to be restructured to better encapsulate data, particularly the deviceStateInfo_t, to improve modularity and reduce tight coupling between components.
- Hardware interactions should be abstracted through the implementation of hardware interfaces, enhancing portability and simplifying potential future hardware migrations.
- The integration of an RTOS is necessary to efficiently manage tasks, improve system responsiveness, and address the limitations of the current single-loop design.

For these adjustments to succeed, test driven design (TDD) should support the entire development process.

## Design of New Architecture (v2.0)

New code has mixture of multiple architecture types, overall architecture could be microkernel (we haven't really done this that well), with ports and adapters or pipes and filters with our extra abstraction layers.

![v2.0 Architecture Diagram](system_diagram_v2.jpg)

### Implementation of Test-Driven-Design

The *Fitness Monitor V1.0* had not implemented any tests throughout the code. This was the most important issue to address initially as with a relatively large software project such as this, issues can pop up quickly from changing small things. Implementing test cases was a preemptive solution to this.
Test cases were written for existing files top-level modules such as `accl_manager`, `display_manager` and `ADC_read`, as well as lower level modules such as 
`circBuf`.
Test files for new modules such as the hardware abstraction modules were implemented _before_ the modules themselves. Applying proper TDD principles and developing a range of test cases _first_ allowed for early bug detection and maintainability.


### Design of FreeRTOS tasking

In the *Fitness Monitor V2.0* architecture, FreeRTOS is employed to manage the scheduling and execution of tasks in a more organized and responsive manner. The key tasks implemented under this design include the superloop task, prompt-to-move task, and read temperature task. Each task is designed with specific responsibilities and priority levels to ensure efficient operation of the fitness monitor. 

#### Superloop Task

The superloop task contains calls to all original V1.0 program functionality. It is responsible for managing core functionalities such as polling inputs, processing accelerometer data, updating the display, and sending data over serial communication (when enabled). It operates continuously in a loop, checking for time intervals to execute its various subtasks. 
- Polling Inputs: The superloop periodically polls buttons and the potentiometer to capture user interactions. The timing for these polls is managed by checking against a system tick counter (`currentTick`), ensuring that inputs are processed at regular intervals defined by `RATE_IO_HZ`.
- Accelerometer Processing: The task also handles the accelerometer data processing. It calculates the magnitude of the accelerometer vector and determines if a step has been detected. This processing is critical for the step counting functionality of the fitness monitor. The task also monitors whether the user has reached their goal, triggering a flash message if the goal is achieved.
- Display Update: Another critical function of the superloop is updating the display at regular intervals, ensuring that the information presented to the user (e.g., steps taken, elapsed time) is current. The display update is handled in a manner that integrates seamlessly with the device's state, including handling flash messages when necessary.
- Serial Communication: If serial plotting is enabled, the superloop also handles the sending of data via USB at specified intervals. This functionality is critical for logging and external monitoring of the device's operation.

#### Prompt-to-Move Task
The prompt-to-move task is a periodic task triggered by a hardware timer interrupt. This task serves as a reminder to the user to stay active by prompting them to move after a set period of inactivity - _this period can be set in `prompt_to_move.c`_.
The timer is initialized to generate interrupts at a specified interval, configured via the `timer_hal_init` function. Upon each interrupt, the corresponding interrupt service routine (ISR) clears the interrupt and notifies the prompt-to-move task. 
Once notified, the task executes the `act_on_prompt_to_move` function, which could involve displaying a message on the screen or triggering an alert to prompt the user to get moving.

#### Read-Temperature Task
The read temperature task is responsible for periodically reading the device's temperature sensor and storing the results for use by other system components, such as the display manager.
This task uses a FreeRTOS queue to store the most recent temperature reading. The temperature is read via the temp_hal_read function and then sent to the queue (more depth in section: I<sup>2</sup>C Implementation). Other tasks, such as those responsible for updating the display, can retrieve the temperature data from this queue when needed.
The task runs continuously in a loop, periodically reading the temperature sensor and updating the queue. It is designed with a relatively high priority to ensure that the temperature readings are kept current, which is especially important for applications where temperature monitoring is critical.


### I<sup>2</sup>C Implementation

The I<sup>2</sup>C bus is implemented through a hardware abstraction layer. There are two tasks using the bus to acquire data from the IMU and the temperature sensor. The following swimlane diagram shows that the two tasks "superloop" and "read_temp" are synchronized through a blocking queue with one entry and thus the I<sup>2</sup>C hardware is protected from double access. For the sake of simplicity only the functions concerning the I<sup>2</sup>C bus are shown.

![Task which use the I2C Bus](i2c_task_distribution.jpg)

This implementation is tested and doesn't lead to any issues in this implementation. However, it must be noted that this isn't a robust implementation, especially if the codebase is refactored and updated in the future. To better protect the hardware from simultaneous access, a mutex in the I<sup>2</sup>C module should be introduced.

### Display Implementation

The design of the display modules follows a modular architecture where each module is responsible for a specific set of tasks. The primary modules involved are:

- `display_manager.c`: Handles the overall display update logic based on the device's state.
- `display_hal.c`: Provides hardware abstraction for display operations, allowing the display manager to work with different hardware without modification.
- `display_helpers.c`: Contains utility functions for rendering text and values on the display, supporting different alignment options and formatting.

The display manager file handles the most basic commands to the display, initialize and update.
The `displayInit` function initializes the display subsystem by calling the hardware abstraction layer's initialization function `display_hal_init`.
The `displayUpdate` is called periodically to update the screen based on the current state of the device.

These functions call the `display_helpers` functions `displayLine`, `displayValue`, and `displayTime`. These are general purpose functions that 'draw' onto the screen. 
These general purpose functions call the hardware abstraction layer `display_hal` functions. The purpoe of this file is to allow the software to be modified minimally in order to change hardware. The functions within this module 

The system is designed with a clear separation of concerns. Each module is responsible for a specific aspect of the display management process, which makes the system easier to maintain, extend, and test. For example, the HAL layer can be modified to support a different display type without changing the higher-level display logic.

Here is an excerpt from the hal file showing how the program could be modified for alternative hardware (for future developers):
```c
// Example HAL Layer Function
void display_hal_init(void) {
    switch (CURRENT_BOARD) {
        case TIVA_BOARD:
            OLEDInitialise();
            break;
        case ALTERNATIVE_HARDWARE:
            break;
        default:
            break;
    }
}
```


## Conclusion
Summarize the key aspects of the new architecture and design improvements.

## References
- [Doxygen Documentation](https://www.doxygen.nl/)

## Appendices
Additional detailed information.
