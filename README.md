# STM32-Based-ATMEL-24RF08-EEPROM-Interface
This repository includes a port of the Arduino-based ThinkPad supervisor password recovery tool to the STM32 range of microcontrollers. {See: https://forum.arduino.cc/t/thinkpad-supervisor-password-read-clear }. All credit goes to the original author for providing the fundamental algorithm.

The program is primarily intended to recover the supervisor password of any ThinkPad laptop that uses the ATMEL 24RF08 EEPROM to store BIOS data. The MCU is commanded over a UART interface to initialize the I2C transactions. This method has been used to successfully recover a supervisor password on an IBM ThinkPad R50e.

This STM32 port is preferable if you already have access to an STM32-NUCLEO microcontroller or if you do not want to include any external circuitry (i.e. I2C pull-up resistors and/or level shifting circuitry).

### Important: The code, and instructions that follow is specifically for the STM32-NUCLEO F411RE (and should work with most MCUs in the NUCLEO line-up). For other series of MCUs, the .IOC file may need to be modified to correct pin mapping discrepancies, and may require further configuration for the UART functionality (Ex. for the discovery series, the USB-OTG-FS needs to be configured to enable UART communication via the micro-USB type B port).

Usage options:
- Read the Password in Plaintext (recommended)
- Read the Password in Plaintext (alternate Scancode)
- Dump the EEPROM in HEX format (for import in IBMpass)
- Dump the EEPROM in Text format (for debug)\
- Clear the password

Requirements:
- STM32 Microcontroller (Recommendation: STM32-NUCLEO F411RE or similar)
- Terminal emulator (Ex. Termite, Tera Term, puTTY, etc.)
- Jumper wires
- STM32CubeIDE

NOTE: It is recommended to use version 1.19.0 of STM32CubeIDE or earlier, as STM32CubeMX and ST-MCU-FINDER-PC are detached from later versions. These components are required and will have to be installed individually if you decide to utilize a later version.

Setup:
- Ensure that you have full access to the 24RF08 EEPROM on your target device
- Import the STM32 project and upload the firmware to your MCU
- Start up and configure the terminal emulator. Reset the microcontroller and ensure that an output is displayed
- Connect the SCL, SDA and GND pins of the 24RF08 EEPROM directly to the corresponding header pins on the microcontroller (Note: PB9 and PB8 is mapped to SDA and SCL, respectively). The EEPROM datasheet and NUCLEO pin reference may be useful at this stage:
    - https://www.alldatasheet.com/datasheet-pdf/download/152541/ATMEL/AT24RF08CN-10SC.html
    - https://deepbluembedded.com/nucleo-f411re-pinout-datasheet-schematic/
- Ensure continuity between connection points and that there are no shorts
- Reset the microcontroller once more and power on the laptop. After a few seconds, choose an option from the list to proceed with the recovery.

<img width="2753" height="3887" alt="20261121_133649" src="https://github.com/user-attachments/assets/36106e43-84c8-4926-8295-928d89fcc777" />

Notice: This project is to be used at the user's own risk. I will not be held responsible for any damage caused to your device(s).

