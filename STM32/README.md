# STM32 – Embedded Systems and IoT Laboratory

This section contains STM32-based laboratory experiments covering embedded C programming, microcontroller peripherals, sensor interfacing and communication protocols.

The experiments are organized by category, with individual task folders containing the firmware source code, project configuration files, circuit diagrams and experiment documentation.

## Hardware and Development Environment

The STM32 experiments use the following microcontrollers and development tools:

| Component | Details |
|---|---|
| STM32F103C8T6 | Used in GPIO, UART, ESP8266 and HTTP server experiments |
| STM32F401RET6 | Used with the NUCLEO-F401RE development board |
| Development tools | STM32CubeMX and Keil µVision |
| Programming language | Embedded C |
| Wi-Fi module | ESP8266 |
| Environmental sensor | BME280 |

**Note:** CAT14 is configured for the STM32F103C8T6 target. The exact physical carrier board used for that experiment has not been confirmed.

## Experiments

| Category | Experiment Area | Tasks |
|---|---|---|
| [CAT8](./CAT8/) | GPIO input/output, switch detection and LED control | TASK8A, TASK8B, TASK8C |
| [CAT9](./CAT9/) | UART character communication and command-based LED control | TASK9A, TASK9B |
| [CAT10](./CAT10/) | STM32–Wemos D1 Mini communication and ESP8266 AT commands | TASK10A, TASK10B, TASK10C |
| [CAT11](./CAT11/) | ESP8266 TCP server and HTTP webpage implementation | TASK11A, TASK11B |
| [CAT12](./CAT12/) | NUCLEO-F401RE onboard LED and user-button interfacing | TASK12A, TASK12B, TASK12C |
| [CAT13](./CAT13/) | BME280 sensor interfacing using I2C | TASK13 |
| [CAT14](./CAT14/) | BME280 sensor data communication using MQTT | TASK14 |

## Peripheral Configuration and Communication

### GPIO

GPIO experiments cover digital input and output operations, switch-state detection, LED control, software debouncing and short-press/long-press detection.

### UART

UART is used for serial communication between the STM32 microcontroller, ESP8266 Wi-Fi module and PC terminal. The experiments include character echoing, command processing and AT-command communication.

### I2C – BME280

The BME280 sensor is interfaced using I2C1.

| Signal | STM32 Pin |
|---|---|
| SCL | PB6 |
| SDA | PB7 |

The sensor driver is used to initialize the device and acquire environmental measurements.

### ESP8266 and HTTP

The ESP8266 is controlled through UART using AT commands. The HTTP experiments configure a TCP server and demonstrate static and dynamic webpage responses.

### MQTT

The MQTT experiment uses the ESP8266 to connect to an MQTT broker and exchange environmental sensor data using the publish-subscribe communication model.

## Project Structure

```text
STM32/
│
├── CAT8/
│   ├── TASK8A/
│   ├── TASK8B/
│   └── TASK8C/
│
├── CAT9/
│   ├── TASK9A/
│   └── TASK9B/
│
├── CAT10/
│   ├── TASK10A/
│   ├── TASK10B/
│   └── TASK10C/
│
├── CAT11/
│   ├── TASK11A/
│   └── TASK11B/
│
├── CAT12/
│   ├── TASK12A/
│   ├── TASK12B/
│   └── TASK12C/
│
├── CAT13/
│   └── TASK13/
│
├── CAT14/
│   └── TASK14/
│
└── README.md
```

## Experiment Documentation

Each task folder contains its own README documenting:

- Experiment details and objective
- Circuit diagram
- Working principle
- Important code logic and implementation
- Output and observations
- Inference

Refer to the individual task folders for the complete implementation and experiment-specific details.

---

**Focus Areas:** Embedded C · STM32 Peripherals · GPIO · UART · I2C · ESP8266 · HTTP · MQTT · Sensor Interfacing