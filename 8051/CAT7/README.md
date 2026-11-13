# CAT7 – 16×2 LCD Interfacing and Keypad-Based Display Control

CAT7 contains three exercises that build on a 4-bit interface to an HD44780-compatible 16×2 character LCD.

| Task | Experiment | Source |
|---|---|---|
| [TASK7A](./TASK7A/README.md) | LCD initialization and static text display | `task7A_lcd.c` |
| [TASK7B](./TASK7B/README.md) | LCD cursor positioning and dynamic counter display | `task7B_lcd.c` |
| [TASK7C](./TASK7C/README.md) | Keypad-controlled character positioning (keypad scan routine is a placeholder in the current source) | `task7C_keypad_lcd.c` |

## Common Hardware and Pin Configuration
- **Microcontroller:** AT89S52 (8051 family)
- **Clock:** 11.0592 MHz crystal
- **LCD:** 16×2 character LCD, 4-bit data interface
- **LCD RS:** P2.0
- **LCD EN:** P2.1
- **LCD data lines D4–D7:** P2.4–P2.7
- **Keypad port in TASK7C:** P1

The task pages describe the LCD routines and application logic implemented in each source file.