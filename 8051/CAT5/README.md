# CAT5 – Timer 0 and Interrupt-Based Control

CAT5 contains three exercises that use Timer 0 interrupts on the AT89S52 to generate timed digital outputs.

| Task | Experiment | Source |
|---|---|---|
| [TASK5A](./TASK5A/README.md) | LED fading using software PWM-style output | `fade.c` |
| [TASK5B](./TASK5B/README.md) | Variable-frequency square-wave generation | `square_wave.c` |
| [TASK5C](./TASK5C/README.md) | Stepper-motor phase sequencing | `stepper.c` |

## Common Setup
- **Microcontroller:** AT89S52 (8051 family)
- **Clock:** 11.0592 MHz crystal
- **Development environment:** Keil µVision / C51
- **Timer:** Timer 0, Mode 1 (16-bit timer)
- **Timer reload in the source files:** TH0 = 0xFC, TL0 = 0x66

Each task configures Timer 0, enables its interrupt, and starts the timer. The timer interrupt service routine reloads the timer and performs the task-specific update. See the individual task README files for the implementation and circuit diagram.