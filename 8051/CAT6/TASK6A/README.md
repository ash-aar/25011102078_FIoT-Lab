# TASK6A – UART Character Transmission Using 8051

## 1. Experiment Details

**Experiment:** UART serial communication using the 8051 microcontroller  
**Program file:** `task_A.c`

## 2. Objective

To configure the UART peripheral of the 8051 microcontroller and transmit a character string to a connected serial device.

## 3. Circuit Diagram

Refer to the `circuit.png` file in this directory for the circuit diagram.

## 4. Working Principle

The 8051 microcontroller uses its built-in UART peripheral to transmit serial data.

The program initializes the UART communication settings and sends the message:

`Hello from 8051`

The data is transmitted serially through the UART transmit pin and can be received by a connected serial terminal or compatible device.

## 5. Implementation / Important Code Logic

The implementation in `task_A.c` performs the following operations:

1. Initializes the UART peripheral.
2. Configures the serial communication settings.
3. Transmits the string `Hello from 8051`.
4. Uses the UART transmit mechanism to send the message character by character.

## 6. Output and Observations

The message `Hello from 8051` is transmitted through the UART interface.

The transmitted message can be observed using a compatible serial terminal when the hardware is connected and the terminal settings match the program configuration.

## 7. Inference

The experiment demonstrates basic UART initialization and serial data transmission using the 8051 microcontroller.