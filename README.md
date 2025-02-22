# Pico Room Monitor

## About

This is a project involving:

- A Raspberry Pi Pico H
- A SCD41 CO2 sensor (with temperature and humidity))
- A TM1637 4-digit 7-segment display
- A red LED
- 4 buttons
- A live/ground rail of a breadboard
- A 3D-printed case

The Pico reads the CO2, temperature and humidity values from the sensor and displays them on the 4-digit display. The LED is turned on when the CO2 level is above a certain threshold. The buttons are used to change the display mode and to set the CO2 threshold.

## Setup

**Step 1:** Connect the components as shown in fritzing/*

**Step 2:** Install the Pico SDK/Toolchain.

**Step 3:** Build and flash the code (Change src/user_variables.h as needed).

```bash
mkdir build
cd build
cmake ..
make
picotool load path/to/roomMonitor.uf2
# Or move the .uf2 file to the Pico manually after putting it BOOTSEL mode
```

## Usage

Press the buttons to change the display mode

1. Temperature
2. Humidity
3. CO2
4. Turn off the display
