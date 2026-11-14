# CAT6 – Serial Communication Using the 8051 UART

CAT6 contains two exercises covering UART initialization, serial transmission, and polling-based reception on the AT89S52.

| Task | Experiment | Source |
|---|---|---|
| [TASK6A](./TASK6A/README.md) | UART initialization and periodic string transmission | `task_A.c` |
| [TASK6B](./TASK6B/README.md) | UART character reception and echo | `task_B.c` |

## Common Setup
- **Microcontroller:** AT89S52 (8051 family)
- **Clock:** 11.0592 MHz crystal
- **Serial interface:** On-chip UART, Mode 1 (8-bit UART)
- **Baud rate:** 9600 baud
- **Timer:** Timer 1, Mode 2 (8-bit auto-reload)

Both programs configure the UART using Timer 1 and communicate through the 8051 serial buffer register (SBUF). See each task page for its specific transmit or receive logic.