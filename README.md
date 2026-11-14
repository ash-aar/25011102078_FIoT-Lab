# Foundations of IoT – Laboratory Work

This repository contains my laboratory work for the **Fundamentals of Internet of Things (IoT)** course as part of my B.Tech in Computer Science and Engineering (IoT).

It documents practical implementations involving microcontrollers, embedded systems, hardware interfacing, sensor communication and IoT-related technologies. The repository includes experiments based on the **8051 microcontroller** and **STM32 microcontrollers**, with source code, circuit diagrams and experiment documentation.

## Repository Overview

| Section | Description |
|---|---|
| [8051](./8051/) | Laboratory experiments involving the 8051 microcontroller |
| [STM32](./STM32/) | STM32-based experiments covering GPIO, UART, sensor interfacing and IoT communication |

## Technologies and Concepts

The laboratory work covers the following areas:

- **Microcontrollers:** 8051 and STM32
- **Embedded C Programming:** Firmware development and peripheral control
- **GPIO:** Digital input and output operations
- **UART:** Serial communication between microcontrollers and external modules
- **I2C:** Interfacing environmental sensors
- **Sensors:** BME280 temperature, pressure and humidity sensor
- **ESP8266:** Wi-Fi connectivity and AT-command communication
- **HTTP:** Basic HTTP response generation and server implementation
- **MQTT:** Publish-subscribe messaging for IoT communication

*The technologies listed above represent the documented STM32 work and the overall areas covered by this repository. Individual experiment details are available in their respective folders.*

## Repository Structure

```text
IoT_GITHUB_REPO/
│
├── 8051/
│   └── README.md
│
├── STM32/
│   ├── README.md
│   │
│   ├── CAT8/
│   ├── CAT9/
│   ├── CAT10/
│   ├── CAT11/
│   ├── CAT12/
│   ├── CAT13/
│   ├── CAT14/
│   └── CAT15/
│
├── .gitignore
├── LICENSE
└── README.md
```

Each STM32 task folder contains its corresponding source files, circuit diagram and `README.md` documenting the experiment.

## STM32 Laboratory Experiments

The STM32 section contains experiments organized by category.

| Category | Area of Work | Tasks |
|---|---|---|
| CAT8 | GPIO input/output and button-based LED control | TASK8A–TASK8C |
| CAT9 | UART communication and command-based LED control | TASK9A–TASK9B |
| CAT10 | STM32–ESP8266 UART communication and AT commands | TASK10A–TASK10C |
| CAT11 | ESP8266-based static and dynamic HTTP server | TASK11A–TASK11B |
| CAT12 | NUCLEO-F401RE GPIO and button interfacing | TASK12A–TASK12C |
| CAT13 | BME280 sensor interfacing using I2C | TASK13 |
| CAT14 | BME280 sensor data communication using MQTT | TASK14 |
| CAT15 | Additional STM32 laboratory project | Refer to the CAT15 folder |

For detailed objectives, working principles, implementation details, circuit diagrams and observations, refer to the individual task documentation.

## Documentation and Circuit Diagrams

Each documented experiment includes:

- Experiment details and objective
- Circuit diagram
- Working principle
- Important implementation logic
- Output and observations
- Inference

Circuit diagrams are provided as PNG images within their respective task folders.

## Purpose

This repository serves as a technical record of my practical work in embedded systems and IoT. It also provides a structured reference for the microcontroller configurations, hardware interfaces, communication protocols and firmware implementations explored during the laboratory course.

---

**Course:** Fundamentals of Internet of Things (IoT)  
**Program:** B.Tech – Computer Science and Engineering (IoT)