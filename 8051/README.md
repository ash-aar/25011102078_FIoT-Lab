# 8051 Microcontroller Lab

This section contains the 8051 microcontroller experiments completed as part of the Fundamentals of IoT laboratory. The exercises use Embedded C to work with GPIO, LED patterns, matrix keypad input, seven-segment output, timer interrupts, UART communication, and character LCD interfacing.

## Hardware and Development Environment

- **Microcontroller:** AT89S52 (8051 family)
- **Development board:** AMOTECH LABS 8051 Universal Development Board
- **Clock:** 11.0592 MHz crystal
- **Programming language:** Embedded C
- **IDE/toolchain:** Keil µVision with the C51 compiler
- **Programming method:** ISP
- **Board supply:** USB Type-C powered development board

The exact peripherals and pin connections used in each experiment are listed in its individual README.

## Experiments

| Assessment | Experiment | Main concepts |
|---|---|---|
| [CAT2](./CAT2/TASK2B/README.md) | 8051 toolchain setup and LED blinking | GPIO output, software delay |
| [CAT3](./CAT3/TASK3A/README.md) | LED operating modes and pattern control | Button polling, debouncing, state-based control |
| [CAT4](./CAT4/TASK4A/README.md) | Matrix keypad, LED and seven-segment interfacing | Keypad scanning, lookup tables, GPIO |
| [CAT5](./CAT5/README.md) | Timer 0 and interrupt-based control | LED PWM-style output, square-wave generation, stepper phase sequencing |
| [CAT6](./CAT6/README.md) | Serial communication using UART | UART initialization, transmission, reception and echo |
| [CAT7](./CAT7/README.md) | 16×2 LCD interfacing and keypad-based display control | 4-bit LCD interface, cursor addressing, dynamic display |

## Project Contents

Each task folder contains its Embedded C source file, Keil project file, circuit diagram, and available build output files. The task README explains the objective, circuit, working principle, implementation, observed output, and inference.

Open the individual task pages from the assessment index above for the details of each experiment.