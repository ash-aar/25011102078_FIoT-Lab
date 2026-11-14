# 8051 – Microcontroller Laboratory

This section contains laboratory experiments based on the **8051 microcontroller**, focusing on embedded C programming, digital output control, waveform generation, serial communication and peripheral interfacing.

The experiments are organized by category and task. Each task folder includes its C source code, Keil µVision project file, circuit diagram and individual experiment documentation.

## Development Environment

| Component | Details |
|---|---|
| Microcontroller | 8051 family |
| Programming language | Embedded C |
| IDE | Keil µVision |
| Project format | Keil µVision project files (`.uvproj`) |
| Assembly support | Startup assembly file (`STARTUP.A51`) |

## Experiments

| Category | Experiment Area | Tasks |
|---|---|---|
| [CAT2](./CAT2/) | LED blinking | TASK2B |
| [CAT3](./CAT3/) | LED operating modes | TASK3A |
| [CAT4](./CAT4/) | Matrix and seven-segment interfacing | TASK4A |
| [CAT5](./CAT5/) | LED fading, square-wave generation and stepper motor control | TASK5A, TASK5B, TASK5C |
| [CAT6](./CAT6/) | UART initialization and serial communication | TASK6A, TASK6B |
| [CAT7](./CAT7/) | LCD interfacing and keypad-based LCD operations | TASK7A, TASK7B, TASK7C |

## Areas Covered

### Digital Output Control

The initial experiments cover LED blinking and different LED operating modes, introducing digital output control through microcontroller programming.

### Display Interfacing

The experiments include matrix and seven-segment interfacing, followed by LCD-based tasks.

### Signal Generation and Motor Control

CAT5 includes implementations involving LED fading, square-wave generation and stepper motor control.

### Serial Communication

CAT6 covers UART initialization and serial data communication, including an echo-based implementation.

### Keypad and LCD Interfacing

CAT7 contains LCD-related experiments, including a task involving keypad and LCD interfacing.

## Project Structure

```text
8051/
│
├── CAT2/
│   └── TASK2B/
│
├── CAT3/
│   └── TASK3A/
│
├── CAT4/
│   └── TASK4A/
│
├── CAT5/
│   ├── TASK5A/
│   ├── TASK5B/
│   └── TASK5C/
│
├── CAT6/
│   ├── TASK6A/
│   └── TASK6B/
│
├── CAT7/
│   ├── TASK7A/
│   ├── TASK7B/
│   └── TASK7C/
│
└── README.md
```

## Experiment Documentation

Each task folder contains its own `README.md`, providing experiment-specific information such as:

- Experiment details and objective
- Circuit diagram
- Working principle
- Important code logic and implementation
- Output and observations
- Inference

The source code and Keil project files are included alongside the documentation to make each experiment easier to inspect and reproduce.

---

**Focus Areas:** 8051 Microcontroller · Embedded C · GPIO · LED Control · Signal Generation · Stepper Motor Control · UART · LCD · Keypad Interfacing