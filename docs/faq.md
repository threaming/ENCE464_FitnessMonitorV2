# Frequently asked questions and errors

[[_TOC_]]

## How do I do *x* in CMake?

See the [CMake guide](./cmake.md) for how to achieve common CMake tasks.

## FreeRTOS

### Why is my program hanging?

It could be in an exception state. For example:

* A hard fault may have occurred, in which case the program will be stuck in
  `FaultISR`.
* A FreeRTOS assertion may have failed, in which case the program will be stuck
  in `vAssertCalled`.

You can use the debugger to determine whether your program is in any of these
states to help narrow things down. In `vAssertCalled`, the filename and line
number where the assertion failed is passed to the function. Debugging hard
faults is a little trickier, you can try viewing the program backtrace in the
debugger. You can also read
[FreeRTOS's debugging tips](https://www.freertos.org/Debugging-Hard-Faults-On-Cortex-M-Microcontrollers.html)
if you are really stuck.

### Why is `vAssertCalled` being called in my ISR?

This can occur if you are using FreeRTOS functions inside an ISR. Make sure you
always use the `*FromISR` variants of the FreeRTOS functions inside ISRs.

If you are doing this and the error still occurs, check you have set the
priority *of the interrupt* to be higher than
`configMAX_SYSCALL_INTERRUPT_PRIORITY`. The `pdTM4C_RTOS_INTERRUPT_PRIORITY`
macro is provided for this purpose - see `FreeRTOSConfig.h`. There are a few
important things to keep in mind when using interrupts with FreeRTOS:

* For hardware interrupts, the higher the number the lower the priority.
* For FreeRTOS tasks, the higher the number the higher the priority.
* Interrupts always have higher priority than RTOS tasks, i.e., an interrupt
  will always preempt a running task.

For more info, read
[FreeRTOS's guide on using the RTOS with Cortex-M cores](https://www.freertos.org/RTOS-Cortex-M3-M4.html).
