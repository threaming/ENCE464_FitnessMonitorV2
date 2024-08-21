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

The purpose of this project was to further develop the current *Fitness Monitor V1.0* embedded software project. We have been instructed to start from the version 1.0 software, and incrementally improve and enhance it. 
The present documentation summarises the findings and development of a imporved version *Fitness Monitor V2.0*. First a analysis of the software version 1.0 is done and key areas of improvements highlighted. After that, the improved architecture of V2.0 is presented and detailed implementations of different components such as FreeRTOS or I<sup>2</sup>C are discussed. A short reflection on the involvement of static and dynamic analysis methods, a section on future improvements and a conclusion close the report.

## Analysis of Existing Architecture

The architecture of the Fitness Monitor v1.0 is clearly outlined in the provided diagram. The system is divided into several key modules, each responsible for a specific aspect of the overall functionality. The diagram uses a color-coded legend to differentiate between standard libraries, leaf modules or libraries, and components requiring further investigation. Key modules include `step_counter_main`, `display_manager`, `acc_manager`, `ADC_read`, `button_manager`, and several others that interact to form the system.

Each module's structure is depicted using class diagrams, showing the functions and attributes within each class. For example, the `step_counter_main` module includes functions like `initClock()`, `initDisplay()`, and `readCurrentTick()`, among others. This diagram provides a clear overview of how different components interact and the roles they play in the system.

![v1.0 Architecture Diagram](system_diagram_v1.jpg)

The v1.0 dependencies depicted in the diagram illustrate how different modules interact with each other. The `step_counter_main` module serves as the central hub, interacting with nearly all other modules. For example, it communicates with the `display_manager` to update the display and with the `acc_manager` to process accelerometer data.

`display_manager` depends directly on the `OrbiOLEDInterface` for hardware-specific operations. `acc_manager` interacts directly with a circular buffer (`circBuf`) to manage accelerometer data and uses the `i2c_driver` for communication with the accelerometer chip. `ADC_read` interacts with `step_counter_main` to provide ADC data when required. `button_manager` and `switches` interact to manage user inputs, which are then relayed to the main module. This dependency structure shows a layered architecture where a single high-level module (`step_counter_main`) coordinates the actions of hardware-specific low level modules.

### Potential Issues

From a maintenance point of view there are already several design issues which have to be addressed. First, there is a circular dependency from the two modules `display_manager` and `button_manager` back to the main module `step_count_main` through the type definition `deviceStateInfo_t`. In other words, the submodules need to know the state of the whole application to work, which prevents interchangeability and reuse of these modules. 

Another critical flaw is the direct interaction of most modules with hardware-specific libraries. For example, the `acc_manager` and `ADC_read` modules directly interface with the accelerometer and ADC hardware, respectively, without any abstraction layer. This design choice tightly couples the software with the specific hardware, reducing the system's portability to other platforms.
The absence of a hardware abstraction layer (HAL) means that any attempt to migrate the system to a new hardware platform would require extensive rework. This design also complicates testing since hardware-specific dependencies cannot be easily mocked or simulated.

Lastly, the core logic or "intelligence" of the application is concentrated within a single loop in the step_counter_main module. This design not only complicates the maintenance of the code but also obscures the overall structure and flow of the application, making it difficult to understand and extend. The absence of any testing framework further exacerbates this issue, as it becomes challenging to verify the correctness of individual modules or identify potential breakages during refactoring. To address this, the introduction of an RTOS (Real-Time Operating System) in the subsequent version has been employed to distribute tasks more effectively, improving code organization, maintainability, and scalability.



## Summary of software design problem
Specifies the v2.0 design (Superficially/Sufficiently/Comprehensively)

After extensive investigation into the *Fitness Monitor V1.0* software design, a few key sections demanded immediate attention.
- The application needs to be restructured to better encapsulate data, particularly the deviceStateInfo_t, to improve modularity and reduce tight coupling between components.
- Hardware interactions should be abstracted through the implementation of hardware interfaces, enhancing portability and simplifying potential future hardware migrations.
- The integration of an RTOS is necessary to efficiently manage tasks, improve system responsiveness, and address the limitations of the current single-loop design.

For these adjustments to succeed, test driven design (TDD) should support the entire development process.

## Design of New Architecture (v2.0)

New code has mixture of multiple architecture types, overall architecture could be microkernel (we haven't really done this that well), with ports and adapters or pipes and filters with our extra abstraction layers.

![v2.0 Architecture Diagram](system_diagram_v2.jpg)


### Design of FreeRTOS tasking
Describe and justify the task architecture.

<!-- ### Example Code Snippet
```c
// Example FreeRTOS task creation
xTaskCreate(TaskFunction, "TaskName", stackSize, NULL, priority, &taskHandle);

![Class Diagram](path_to_class_diagram.png) -->

### I<sup>2</sup>C Implementation

The I<sup>2</sup>C bus is implemented through a hardware abstraction layer. There are two tasks using the bus to acquire data from the IMU and the temperature sensor. The following swimlane diagram shows that the two tasks "superloop" and "read_temp" are synchronized through a blocking queue with one entry and thus the I<sup>2</sup>C hardware is protected from double access. For the sake of simplicity only the functions concerning the I<sup>2</sup>C bus are shown.

![Task which use the I2C Bus](i2c_task_distribution.jpg)

This implementation is tested and doesn't lead to any issues in this implementation. However, it must be noted that this isn't a robust implementation, especially if the codebase is refactored and updated in the future. To better protect the hardware from simultaneous access, a mutex in the I<sup>2</sup>C module should be introduced.

## Application of static and dynamic analysis
Analyses aspects of the design to guide decisions (Poorly/Satisfactorily/Proficiently)


## Usefulness of documentation to future developers
Explains the existing design and how to modify it (Superficially/Sufficiently/Comprehensively)



## Conclusion
Summarize the key aspects of the new architecture and design improvements.

## References
- [Doxygen Documentation](https://www.doxygen.nl/)

## Appendices
Additional detailed information.
