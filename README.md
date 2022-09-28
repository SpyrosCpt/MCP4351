# MCP4351
Arduino Library for MCP4351 Digital Potentiometer

**About**

This library can be used for interfacing and controlling the MCP4351 IC with the Arduino. The MCP4351 IC is a volatile, 8-bit (257 wiper steps) digital potentiometers with an SPI compatible interface. It is available with end-to-end resistor values of 5KΩ, 10KΩ, 50kΩ and 100KΩ. More information on the product page.Other MCP42xx family ICs can also be used, but full functionality of the library may not be guaranteed.

**Features**

All available controls as per MCP4351 datasheet.
Additional functions like DigitalPotResistanceToPosition and DigitalPotPositionToResistance.
Simple to use.
Multiple MCP4351 ICs can be interfaced and controlled, subjected to the availability of I/O pins on Arduino board.

**Installation**

1. Navigate to the Releases page.
2. Download the latest release.
3. Extract the zip file
4. In the Arduino IDE, navigate to Sketch > Include Library > Add .ZIP Library

**Functions**

Set wiper position
- DigitalPotWiperIncrement
- DigitalPotWiperDecrement
- DigitalPotSetWiperPosition
- DigitalPotSetWiperMin
- DigitalPotSetWiperMax
- DigitalPotSetWiperMid
- DigitalPotReadWiperPosition

Set control registers
- DigitalPotReadTconRegister
- DigitalPotReadStatusRegister
- DigitalPotWriteTcon0Register
- DigitalPotWriteTcon1Register

Enable/disable connections
- DigitalPotStartup
- DigitalPotShutdown
- DigitalPotTerminalBConnect
- DigitalPotTerminalBDisconnect
- DigitalPotTerminalAConnect
- DigitalPotTerminalADisconnect
- DigitalPotWiperConnect
- DigitalPotWiperDisconnect
- DigitalPotInitTcon

Additional
- DigitalPotResistanceToPosition
- DigitalPotPositionToResistance

**Usage**

Two examples are available in library. Following is the simple example of interfacing a single MCP4351 with the Arduino. Only one potentiometer (pot0) is controlled and wiper is set to a position = 256.

connection-diagram

        #include <MCP4351.h>

        #define chipSelectPin 53
        #define pot0ResistanceRmax 10000 // These resistance values may vary
        #define pot0ResistanceRmin 0
        #define pot1ResistanceRmax 10000
        #define pot1ResistanceRmin 0
        #define pot2ResistanceRmax 10000 // These resistance values may vary
        #define pot2ResistanceRmin 0
        #define pot3ResistanceRmax 10000
        #define pot3ResistanceRmin 0

        #define MAX_NUMBER_OF_POTS 4
        #define MAX_POT_POSITION   255

        MCP4351 digitalPot(chipSelectPin, pot0ResistanceRmax, pot0ResistanceRmin, pot1ResistanceRmax, pot1ResistanceRmin, pot2ResistanceRmax, pot2ResistanceRmin, pot3ResistanceRmax, pot3ResistanceRmin);

        uint16_t pot_pos;
        uint8_t pot_num = 0;

        void setup()
        {    
            Serial.begin(9600);
            digitalPot.begin();

            for(pot_num = 0; pot_num < MAX_NUMBER_OF_POTS; pot_num++)
            {
                digitalPot.DigitalPotStartup(pot_num);
            }
        }

        void loop()
        {
            for(pot_pos = 0; pot_pos < MAX_POT_POSITION; pot_pos++)
            {
                for(pot_num = 0; pot_num < MAX_NUMBER_OF_POTS; pot_num++)
                {
                    digitalPot.DigitalPotSetWiperPosition(pot_num, pot_pos);
                }       
                delay(50);
            }
        }

**Documentation**

The documentation is available at https://kulbhushanchand.github.io/MCP4251/

**Contributing**

Any contributions you make are greatly appreciated. You can contribute to this project in the following ways :

- Add new functionality
- Review code
- Raise issues about bugs/features/doubts
- Proof-read the documentation
- Cite if used in a publication
- Star on GitHub
- Share with others
Please note that this project is released with a Contributor Code of Conduct. By contributing to this project you agree to abide by its terms.

**License**

This project is distributed under the GPLv3 License. See LICENSE for more information.

**Acknowledgment**

This project is based off of kulbhushanchand's MCP4251 project that can be found here: https://github.com/kulbhushanchand/MCP4251.
I have just extended this work for the 4 Potentiometer variant.
The open-source tools used in development of this project.

drawio-desktop is used to create logo and diagrams.
Fritzing is used to create the connection diagram.
