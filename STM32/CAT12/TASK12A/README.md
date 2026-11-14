# TASK12A – NUCLEO-F401RE On-board LED Blinking

## 1. Experiment Details

**Board:** NUCLEO-F401RE  
**Microcontroller:** STM32F401RET6  
**On-board LED:** LD2  
**GPIO Pin:** PA5

## 2. Objective

To configure the GPIO pin connected to the on-board LED and implement a simple LED blinking program using the STM32 HAL library.

## 3. Circuit Diagram

Refer to the `circuit.png` file in this directory for the circuit diagram.

The experiment uses the NUCLEO-F401RE board's built-in LED, LD2, connected to PA5.

## 4. Working Principle

The GPIO pin PA5 is configured as a general-purpose push-pull output.

The program repeatedly toggles the output state of PA5 with a delay of 500 milliseconds.

- When PA5 is HIGH, the LED turns ON.
- When PA5 is LOW, the LED turns OFF.

The repeated toggling produces a continuous blinking effect.

## 5. Implementation / Important Code Logic

The implementation in `main.c` follows these steps:

1. Initializes the HAL library and system clock.
2. Configures GPIOA and sets PA5 as an output.
3. Uses `HAL_GPIO_TogglePin()` to change the output state of PA5.
4. Uses `HAL_Delay(500)` to maintain a 500 ms interval between toggles.
5. Repeats the operation continuously inside the main loop.

## 6. Output and Observations

The on-board green LED LD2 blinks continuously.

The LED changes its state every 500 milliseconds, resulting in an approximate full ON-OFF cycle of one second.

## 7. Inference

The experiment demonstrates basic GPIO output configuration and control using the STM32 HAL library. It verifies the operation of the NUCLEO-F401RE's on-board LED through software-controlled GPIO toggling.