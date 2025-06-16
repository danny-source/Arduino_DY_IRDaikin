
# Arduino_DY_IRDaikin ARC/BRC

The **Arduino_DY_IRDaikin ARC/BRC** library is designed to send IR commands to control Daikin air conditioners (ARC/BRC). It is based on Ken Shirriff's IRremote library and has been further developed to support various microcontroller platforms.

---

## Introduction

- **Start Date**: September 2014  
- **Copyright**: 2025 by Danny  
- **Original Name**: Arduino_IRremote_Daikin  
- **Current Name**: Arduino_DY_IRDaikin ARC/BRC  
- **Purpose**: This library enables control of Daikin air conditioners via IR commands, including power on/off, mode switching, temperature adjustment, and more.

---

## Supported Platforms

### Arduino UNO
![Arduino UNO Diagram](https://raw.githubusercontent.com/danny-source/Arduino_IRremote_Daikin/master/Arduino_UNO_IR_Transmit.png)

### Ameba Arduino
![Ameba Arduino Diagram](https://raw.githubusercontent.com/danny-source/Arduino_IRremote_Daikin/master/Arduino_Ameba_IR_Transmit.png)

---

## Changelog

- **1.1.2**  
  - Fixed PWM frequency.  
  - Refactored `DYIRDaikinRecv` class.  

- **1.1.1**  
  - Fixed issues with software IR frequency and wave width.  

- **1.1.0**  
  - Updated class and function names.  
  - Added support for STM32Duino, Arduino ESP8266, Ameba platforms.  
  - Added support for Arduino PWM IR signals.  
  - Added SOFT IR support for Arduino UNO/Nano/Mega 2560.  

- **1.0.2**  
  - Added support for ESP8266/Ameba (software IR) and Arduino PWM IR signals.  

- **1.0.1**  
  - Added support for ESP8266 (software IR) and Arduino PWM IR signals.  

- **1.0.0**  
  - Refactored code.  
  - Added ARC support for IR decoding.  

- **0.0.5**  
  - Renamed `IRremote` to `IRremoteDaikin` for isolation.  
  - Refactored code.  

---

## PWM Pinout

### Arduino UNO
- Pin 3 (D3)

### Arduino Leonardo
- Pin 2 (D2)

### Arduino Mega
- Pin 9 (D9)

---

## SOFT IR Pinout

The default pin for SOFT IR when using `begin()` is pin 3.  
If you need to use a different pin, you must specify it using `begin(int irSendPin)`.

### ESP8266
- Pin 4 (GPIO4)  
- Pin 5 (GPIO5)  

### Ameba RTL8195
- Pin 2 (D2)  
- Pin 3 (D3)  
- Pin 4 (D4)  
- Pin 5 (D5)  
- Pin 6 (D6)  
- Pin 7 (D7)  

### Ameba RTL8710
- Pin 2 (D2)  
- Pin 5 (D5)  

### STM32F1
- Pin 2 (PA2)  
- Pin 3 (PA3)  

### STM32F4
- Pin 2 (PA2)  
- Pin 3 (PA3)  
---

## Daikin ARC Functions

### Basic Functions
- `on();` — Turn on the air conditioner.  
- `off();` — Turn off the air conditioner.  

### Swing Control
- `setSwing_on();` — Enable swing mode.  
- `setSwing_off();` — Disable swing mode.  

### Mode Settings
- `setMode(int mode);` — Set the mode:  
  - `0` = FAN  
  - `1` = COOL  
  - `2` = DRY  
  - `3` = HEAT (if supported by your AC model)  

### Fan Speed
- `setFan(int speed);` — Set fan speed:  
  - `0-4` = Speed levels (1, 2, 3, 4, 5)  
  - `5` = Auto  
  - `6` = Moon mode  

### Temperature
- `setTemp(int temp);` — Set temperature:  
  - 18–32°C for COOL mode  
  - 10–30°C for HEAT mode  
  - 18–30°C for AUTO mode  

### Command Execution
- `sendCommand();` — Send the configured command.  

### Additional Functions
- `decode();` — Decode IR signals.  
- `description();` — Print the current configuration and states.  

---

## Daikin BRC Functions

### Basic Functions
- `on();` — Turn on the air conditioner.  
- `off();` — Turn off the air conditioner.  

### Swing Control
- `setSwing_on();` — Enable swing mode.  
- `setSwing_off();` — Disable swing mode.  

### Fan Speed
- `setFan(int speed);` — Set fan speed:  
  - `0` = Low  
  - `1` = High  

### Temperature
- `setTemp(int temp);` — Set temperature (18–36°C).  

### Command Execution
- `sendCommand();` — Send the configured command.  

---

## How to Execute Commands

To use the library, you must first initialize and configure the settings in your program. After configuring, use `sendCommand()` to execute the command.

---

## Example Code for Daikin ARC

```cpp
begin();              // Initialize the IR library
on();                 // Turn on the air conditioner
setSwing_off();       // Disable swing mode
setMode(1);           // Set to COOL mode
setFan(4);            // Set fan speed to maximum
setTemp(25);          // Set temperature to 25°C
description();        // Print the current configuration
sendCommand();        // Send the IR command


## Acknowledgments

`Kwok Fai Fong` help me build Daikin BRC remote control support.

## Contributors 
`Peter Lambrechts`

## Donate

[![Donate with PayPal](https://raw.githubusercontent.com/stefan-niedermann/paypal-donate-button/master/paypal-donate-button.png)](https://www.paypal.me/dannytwdanny)
