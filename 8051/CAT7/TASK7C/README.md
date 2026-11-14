# TASK7C – Keypad Interfacing with LCD

## 1. Experiment Details

**Experiment:** Keypad interfacing with an LCD using the 8051 microcontroller  
**Program file:** `task7C_keypad_lcd.c`

## 2. Objective

To develop an 8051-based program for interfacing a keypad with an LCD and displaying character information based on keypad input.

## 3. Circuit Diagram

Refer to the `circuit.png` file in this directory for the circuit diagram.

## 4. Working Principle

The experiment is intended to combine keypad input and LCD output.

The keypad scanning routine is responsible for detecting a key press. The detected key can then be processed by the main program and displayed on the LCD.

The LCD is initialized before the main program enters its input-processing loop.

**Implementation status:** The current `scan_keypad()` function is a placeholder that always returns `0`. Therefore, actual keypad detection is not implemented, and the keypad-based character movement functionality does not operate as intended.

## 5. Implementation / Important Code Logic

The implementation contains the following components:

1. LCD initialization and command functions.
2. LCD character display functionality.
3. A keypad scanning function named `scan_keypad()`.
4. Main-loop logic intended to process keypad input and update the LCD display.

The current keypad scanning function returns `0` without detecting any physical key presses. As a result, the main program does not receive actual keypad input.

## 6. Output and Observations

The LCD initialization and display-related functions are present in the program.

However, keypad input detection is incomplete because `scan_keypad()` is currently a placeholder. The intended keypad-based character movement cannot be verified from the current implementation.

## 7. Inference

The experiment demonstrates the intended structure of keypad and LCD interfacing using the 8051 microcontroller. The LCD-related implementation is present, but the keypad scanning routine requires further development before the complete experiment can function as intended.