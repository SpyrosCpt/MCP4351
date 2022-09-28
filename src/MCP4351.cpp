#include "MCP4351.h"
#include <SPI.h>

// Instantiation of object
MCP4351::MCP4351(uint8_t slaveSelectPin, float pot0ResistanceRmax, float pot0ResistanceRmin, float pot1ResistanceRmax, float pot1ResistanceRmin, float pot2ResistanceRmax, float pot2ResistanceRmin, float pot3ResistanceRmax, float pot3ResistanceRmin)
{
    this->_slaveSelectPin = slaveSelectPin;

    this->_pot0ResistanceRmax = pot0ResistanceRmax;
    this->_pot0ResistanceRmin = pot0ResistanceRmin;
    this->_pot0ResistanceRAB = pot0ResistanceRmax - pot0ResistanceRmin;
    this->_pot0ResistanceRW = pot0ResistanceRmin;

    this->_pot1ResistanceRmax = pot1ResistanceRmax;
    this->_pot1ResistanceRmin = pot1ResistanceRmin;
    this->_pot1ResistanceRAB = pot1ResistanceRmax - pot1ResistanceRmin;
    this->_pot1ResistanceRW = pot1ResistanceRmin;

    this->_pot2ResistanceRmax = pot2ResistanceRmax;
    this->_pot2ResistanceRmin = pot2ResistanceRmin;
    this->_pot2ResistanceRAB = pot2ResistanceRmax - pot2ResistanceRmin;
    this->_pot2ResistanceRW = pot2ResistanceRmin;

    this->_pot3ResistanceRmax = pot3ResistanceRmax;
    this->_pot3ResistanceRmin = pot3ResistanceRmin;
    this->_pot3ResistanceRAB = pot3ResistanceRmax - pot3ResistanceRmin;
    this->_pot3ResistanceRW = pot3ResistanceRmin;
}

void MCP4351::begin()
{
    ::pinMode(_slaveSelectPin, OUTPUT);
    ::digitalWrite(_slaveSelectPin, HIGH);
    SPI.begin(18, 19, 23, 5);
    this->DigitalPotInitTcon();
    this->DigitalPotSetWiperMin(0);
    this->DigitalPotSetWiperMin(1);
    this->DigitalPotSetWiperMin(3);
    this->DigitalPotSetWiperMin(4);
}

void MCP4351::DigitalPotWiperIncrement(uint8_t potNum)
{
    byte cmdByte = B00000000;
    ::digitalWrite(_slaveSelectPin, LOW);
    if(potNum == 0)
    {
        cmdByte = ADDRESS_WIPER_0 | COMMAND_INCREMENT;
    }
    else if(potNum == 1)
    {
        cmdByte = ADDRESS_WIPER_1 | COMMAND_INCREMENT;
    }
    if(potNum == 2)
    {
        cmdByte = ADDRESS_WIPER_2 | COMMAND_INCREMENT;
    }
    else if(potNum == 3)
    {
        cmdByte = ADDRESS_WIPER_3 | COMMAND_INCREMENT;        
    }
    SPI.transfer(cmdByte);
    ::digitalWrite(_slaveSelectPin, HIGH);
}

void MCP4351::DigitalPotWiperDecrement(uint8_t potNum)
{
    byte cmdByte = B00000000;
    ::digitalWrite(_slaveSelectPin, LOW);
    if(potNum == 0)
    {
        cmdByte = ADDRESS_WIPER_0 | COMMAND_DECREMENT;
    }
    else if(potNum == 1)
    {
        cmdByte = ADDRESS_WIPER_1 | COMMAND_DECREMENT;
    }
    else if(potNum == 2)
    {
        cmdByte = ADDRESS_WIPER_2 | COMMAND_DECREMENT;
    }
    else if(potNum == 3)
    {
        cmdByte = ADDRESS_WIPER_3 | COMMAND_DECREMENT;
    }
    SPI.transfer(cmdByte);
    ::digitalWrite(_slaveSelectPin, HIGH);
}

void MCP4351::DigitalPotSetWiperPosition(uint8_t potNum, uint16_t value)
{
    byte cmdByte = B00000000;
    byte dataByte = B00000000;
    if (value > 255)
    {
        cmdByte |= B00000001;
    }
    else
    {
        dataByte = (byte)(value & 0X00FF);
    }
    ::digitalWrite(_slaveSelectPin, LOW);
    if(potNum == 0)
    {
        cmdByte = cmdByte | ADDRESS_WIPER_0 | COMMAND_WRITE;        
    }
    else if(potNum == 1)
    {
        cmdByte = cmdByte | ADDRESS_WIPER_1 | COMMAND_WRITE;
    }
    else if(potNum == 2)
    {
        cmdByte = cmdByte | ADDRESS_WIPER_2 | COMMAND_WRITE;        
    }
    else if(potNum == 3)
    {
        cmdByte = cmdByte | ADDRESS_WIPER_3 | COMMAND_WRITE;
    }
    SPI.transfer(cmdByte);
    SPI.transfer(dataByte);
    ::digitalWrite(_slaveSelectPin, HIGH);
}

void MCP4351::DigitalPotSetWiperMin(uint8_t potNum)
{
    DigitalPotSetWiperPosition(potNum, 0);
}

void MCP4351::DigitalPotSetWiperMax(uint8_t potNum)
{
    DigitalPotSetWiperPosition(potNum, 256);
}

void MCP4351::DigitalPotSetWiperMid(uint8_t potNum)
{
    DigitalPotSetWiperPosition(potNum, 128);
}

uint16_t MCP4351::DigitalPotReadWiperPosition(uint8_t potNum)
{
    byte cmdByte = B00000000;
    byte hByte = B00000000;
    byte lByte = B00000000;
    ::digitalWrite(_slaveSelectPin, LOW);
    if(potNum == 0)
    {
        cmdByte = ADDRESS_WIPER_0 | COMMAND_READ;
    }
    else if(potNum == 1)
    {
        cmdByte = ADDRESS_WIPER_1 | COMMAND_READ;
    }
    else if(potNum == 2)
    {
        cmdByte = ADDRESS_WIPER_2 | COMMAND_READ;
    }
    else if(potNum == 3)
    {
        cmdByte = ADDRESS_WIPER_3 | COMMAND_READ;
    }
    hByte = SPI.transfer(cmdByte);
    lByte = SPI.transfer(DUMMY_DATA);
    ::digitalWrite(_slaveSelectPin, HIGH);
    return ((uint16_t)hByte << 8 | (uint16_t)lByte) & BITMASK_READ_DATA_REGISTER;
}

uint16_t MCP4351::DigitalPotReadStatusRegister()
{
    byte cmdByte = B00000000;
    byte hByte = B00000000;
    byte lByte = B00000000;
    ::digitalWrite(_slaveSelectPin, LOW);
    cmdByte = ADDRESS_STATUS | COMMAND_READ;
    hByte = SPI.transfer(cmdByte);
    lByte = SPI.transfer(DUMMY_DATA);
    ::digitalWrite(_slaveSelectPin, HIGH);
    return ((uint16_t)hByte << 8 | (uint16_t)lByte) & BITMASK_READ_DATA_REGISTER;
}

uint16_t MCP4351::DigitalPotReadTconRegister()
{
    byte cmdByte = B00000000;
    byte hByte = B00000000;
    byte lByte = B00000000;
    ::digitalWrite(_slaveSelectPin, LOW);
    cmdByte = ADDRESS_TCON0 | COMMAND_READ;
    hByte = SPI.transfer(cmdByte);
    lByte = SPI.transfer(DUMMY_DATA);
    ::digitalWrite(_slaveSelectPin, HIGH);
    return ((uint16_t)hByte << 8 | (uint16_t)lByte) & BITMASK_READ_DATA_REGISTER;
}

void MCP4351::DigitalPotWriteTcon0Register(uint16_t value)
{
    byte cmdByte = B00000000;
    byte dataByte = B00000000;
    if (value > 255)
    {
        cmdByte |= B00000001;
    }
    else
    {
        dataByte = (byte)(value & 0X00FF);
    }
    ::digitalWrite(_slaveSelectPin, LOW);
    cmdByte = cmdByte | ADDRESS_TCON0 | COMMAND_WRITE;
    SPI.transfer(cmdByte);
    SPI.transfer(dataByte);
    ::digitalWrite(_slaveSelectPin, HIGH);
}

void MCP4351::DigitalPotWriteTcon1Register(uint16_t value)
{
    byte cmdByte = B00000000;
    byte dataByte = B00000000;
    if (value > 255)
    {
        cmdByte |= B00000001;
    }
    else
    {
        dataByte = (byte)(value & 0X00FF);
    }
    ::digitalWrite(_slaveSelectPin, LOW);
    cmdByte = cmdByte | ADDRESS_TCON1 | COMMAND_WRITE;
    SPI.transfer(cmdByte);
    SPI.transfer(dataByte);
    ::digitalWrite(_slaveSelectPin, HIGH);
}

void MCP4351::DigitalPotStartup(uint8_t potNum)
{
    uint16_t tconData = this->DigitalPotReadTconRegister();
    byte hByte = (uint8_t)tconData >> 8;
    byte lByte = (uint8_t)tconData & 0xff;

    if(potNum == 0)
    {
        lByte = lByte | BITMASK_POT0_STARTUP;
    }
    else if(potNum == 1)
    {
        lByte = lByte | BITMASK_POT1_STARTUP;
    }
    else if(potNum == 2)
    {
        lByte = lByte | BITMASK_POT2_STARTUP;
    }
    else if(potNum == 3)
    {
        lByte = lByte | BITMASK_POT3_STARTUP;
    }

    tconData = (uint16_t)hByte << 8 | (uint16_t)lByte;
    if(potNum < 2)
    {
        this->DigitalPotWriteTcon0Register(tconData);
    }
    else
    {
        this->DigitalPotWriteTcon1Register(tconData);
    }
}

void MCP4351::DigitalPotShutdown(uint8_t potNum)
{
    uint16_t tconData = this->DigitalPotReadTconRegister();
    byte hByte = (uint8_t)tconData >> 8;
    byte lByte = (uint8_t)tconData & 0xff;

    if(potNum == 0)
    {
        lByte = lByte & ~BITMASK_POT0_STARTUP;
    }
    else if(potNum == 1)
    {
        lByte = lByte & ~BITMASK_POT1_STARTUP;
    }
    else if(potNum == 2)
    {
        lByte = lByte & ~BITMASK_POT2_STARTUP;
    }
    else if(potNum == 3)
    {
        lByte = lByte & ~BITMASK_POT3_STARTUP;
    }
    tconData = (uint16_t)hByte << 8 | (uint16_t)lByte;
    if(potNum < 2)
    {
        this->DigitalPotWriteTcon0Register(tconData);
    }
    else
    {
        this->DigitalPotWriteTcon1Register(tconData);
    }
}

void MCP4351::DigitalPotTerminalBConnect(uint8_t potNum)
{
    uint16_t tconData = this->DigitalPotReadTconRegister();
    byte hByte = (uint8_t)tconData >> 8;
    byte lByte = (uint8_t)tconData & 0xff;

    if(potNum == 0)
    {
        lByte = lByte | BITMASK_POT0_B_TERMINAL_CONNECT;
    }
    else if(potNum == 1)
    {
        lByte = lByte | BITMASK_POT1_B_TERMINAL_CONNECT;
    }
    else if(potNum == 2)
    {
        lByte = lByte | BITMASK_POT2_B_TERMINAL_CONNECT;
    }
    else if(potNum == 3)
    {
        lByte = lByte | BITMASK_POT3_B_TERMINAL_CONNECT;
    }    

    tconData = (uint16_t)hByte << 8 | (uint16_t)lByte;
    if(potNum < 2)
    {
        this->DigitalPotWriteTcon0Register(tconData);
    }
    else
    {
        this->DigitalPotWriteTcon1Register(tconData);
    }
}

void MCP4351::DigitalPotTerminalBDisconnect(uint8_t potNum)
{
    uint16_t tconData = this->DigitalPotReadTconRegister();
    byte hByte = (uint8_t)tconData >> 8;
    byte lByte = (uint8_t)tconData & 0xff;

    if (potNum == 0)
    {
        lByte = lByte & ~BITMASK_POT0_B_TERMINAL_CONNECT;
    } 
    else if(potNum == 1)
    {
        lByte = lByte & ~BITMASK_POT1_B_TERMINAL_CONNECT;
    }
    else if (potNum == 2)
    {
        lByte = lByte & ~BITMASK_POT2_B_TERMINAL_CONNECT;
    } 
    else if(potNum == 3)
    {
        lByte = lByte & ~BITMASK_POT3_B_TERMINAL_CONNECT;
    }

    tconData = (uint16_t)hByte << 8 | (uint16_t)lByte;
    if(potNum < 2)
    {
        this->DigitalPotWriteTcon0Register(tconData);
    }
    else
    {
        this->DigitalPotWriteTcon1Register(tconData);
    }
}

void MCP4351::DigitalPotTerminalAConnect(uint8_t potNum)
{
    uint16_t tconData = this->DigitalPotReadTconRegister();
    byte hByte = (uint8_t)tconData >> 8;
    byte lByte = (uint8_t)tconData & 0xff;

    if(potNum == 0)
    {
        lByte = lByte | BITMASK_POT0_A_TERMINAL_CONNECT;
    }
    else if(potNum == 1)
    {
        lByte = lByte | BITMASK_POT1_A_TERMINAL_CONNECT;
    }
    else if(potNum == 2)
    {
        lByte = lByte | BITMASK_POT2_A_TERMINAL_CONNECT;
    }
    else if(potNum == 3)
    {
        lByte = lByte | BITMASK_POT3_A_TERMINAL_CONNECT;
    }

    tconData = (uint16_t)hByte << 8 | (uint16_t)lByte;
    if(potNum < 2)
    {
        this->DigitalPotWriteTcon0Register(tconData);
    }
    else
    {
        this->DigitalPotWriteTcon1Register(tconData);
    }
}

void MCP4351::DigitalPotTerminalADisconnect(uint8_t potNum)
{
    uint16_t tconData = this->DigitalPotReadTconRegister();
    byte hByte = (uint8_t)tconData >> 8;
    byte lByte = (uint8_t)tconData & 0xff;

    if(potNum == 0)
    {
        lByte = lByte & ~BITMASK_POT0_A_TERMINAL_CONNECT;
    }
    else if(potNum == 1)
    {
        lByte = lByte & ~BITMASK_POT1_A_TERMINAL_CONNECT;
    }
    else if(potNum == 2)
    {
        lByte = lByte & ~BITMASK_POT2_A_TERMINAL_CONNECT;
    }
    else if(potNum == 3)
    {
        lByte = lByte & ~BITMASK_POT3_A_TERMINAL_CONNECT;
    }

    tconData = (uint16_t)hByte << 8 | (uint16_t)lByte;
    if(potNum < 2)
    {
        this->DigitalPotWriteTcon0Register(tconData);
    }
    else
    {
        this->DigitalPotWriteTcon1Register(tconData);
    }
}

void MCP4351::DigitalPotWiperConnect(uint8_t potNum)
{
    uint16_t tconData = this->DigitalPotReadTconRegister();
    byte hByte = (uint8_t)tconData >> 8;
    byte lByte = (uint8_t)tconData & 0xff;

    if(potNum == 0)
    {
        lByte = lByte | BITMASK_POT0_WIPER_TERMINAL_CONNECT;
    }
    else if(potNum == 1)
    {
        lByte = lByte | BITMASK_POT1_WIPER_TERMINAL_CONNECT;
    }
    if(potNum == 2)
    {
        lByte = lByte | BITMASK_POT2_WIPER_TERMINAL_CONNECT;
    }
    else if(potNum == 3)
    {
        lByte = lByte | BITMASK_POT3_WIPER_TERMINAL_CONNECT;
    }

    tconData = (uint16_t)hByte << 8 | (uint16_t)lByte;
    if(potNum < 2)
    {
        this->DigitalPotWriteTcon0Register(tconData);
    }
    else
    {
        this->DigitalPotWriteTcon1Register(tconData);
    }
}

void MCP4351::DigitalPotWiperDisconnect(uint8_t potNum)
{
    uint16_t tconData = this->DigitalPotReadTconRegister();
    byte hByte = (uint8_t)tconData >> 8;
    byte lByte = (uint8_t)tconData & 0xff;

    if(potNum == 0)
    {
        lByte = lByte & ~BITMASK_POT0_WIPER_TERMINAL_CONNECT;
    }
    else if(potNum == 1)
    {
        lByte = lByte & ~BITMASK_POT1_WIPER_TERMINAL_CONNECT;
    }
    else if(potNum == 2)
    {
        lByte = lByte & ~BITMASK_POT2_WIPER_TERMINAL_CONNECT;
    }
    else if(potNum == 3)
    {
        lByte = lByte & ~BITMASK_POT3_WIPER_TERMINAL_CONNECT;
    }


    tconData = (uint16_t)hByte << 8 | (uint16_t)lByte;
    if(potNum < 2)
    {
        this->DigitalPotWriteTcon0Register(tconData);
    }
    else
    {
        this->DigitalPotWriteTcon1Register(tconData);
    }
}

void MCP4351::DigitalPotInitTcon()
{
    uint16_t tconData = this->DigitalPotReadTconRegister();
    byte hByte = (uint8_t)tconData >> 8;
    byte lByte = (uint8_t)tconData & 0xff;

    lByte = lByte | DUMMY_DATA;

    tconData = (uint16_t)hByte << 8 | (uint16_t)lByte;
    this->DigitalPotWriteTcon0Register(tconData);
    this->DigitalPotWriteTcon1Register(tconData);
    
}

uint16_t MCP4351::DigitalPotResistanceToPosition(uint8_t potNum, float resistance)
{
    if(potNum == 0)
    {
        if (resistance <= this->_pot0ResistanceRmin)
        {
            return 0;
        }
        else if (resistance > this->_pot0ResistanceRmax)
        {
            return 256;
        }
        return (uint16_t)((((resistance - this->_pot0ResistanceRW) / (this->_pot0ResistanceRAB)) * (float)256) + 0.5);
    }
    else if(potNum == 1)
    {
        if (resistance <= this->_pot1ResistanceRmin)
        {
            return 0;
        }
        else if (resistance > this->_pot1ResistanceRmax)
        {
            return 256;
        }
        return (uint16_t)((((resistance - this->_pot1ResistanceRW) / (this->_pot1ResistanceRAB)) * (float)256) + 0.5);
    }
    else if(potNum == 2)
    {
        if (resistance <= this->_pot2ResistanceRmin)
        {
            return 0;
        }
        else if (resistance > this->_pot2ResistanceRmax)
        {
            return 256;
        }
        return (uint16_t)((((resistance - this->_pot2ResistanceRW) / (this->_pot2ResistanceRAB)) * (float)256) + 0.5);
    }
    else
    {
        if (resistance <= this->_pot3ResistanceRmin)
        {
            return 0;
        }
        else if (resistance > this->_pot3ResistanceRmax)
        {
            return 256;
        }
        return (uint16_t)((((resistance - this->_pot3ResistanceRW) / (this->_pot3ResistanceRAB)) * (float)256) + 0.5);
    }
}

float MCP4351::DigitalPotPositionToResistance(uint8_t potNum, uint16_t position)
{
    if(potNum == 0)
    {
        if (position < 0)
        {
            return this->_pot0ResistanceRmin;
        }
        else if (position > 256)
        {
            return this->_pot0ResistanceRmax;
        }
        return ((this->_pot0ResistanceRAB / 256) * (float)position) + this->_pot0ResistanceRW;
    }
    else if(potNum == 1)
    {
        if (position < 0)
        {
            return this->_pot1ResistanceRmin;
        }
        else if (position > 256)
        {
            return this->_pot1ResistanceRmax;
        }
        return ((this->_pot1ResistanceRAB / 256) * (float)position) + this->_pot1ResistanceRW;
    }
    else if(potNum == 2)
    {

        if (position < 0)
        {
            return this->_pot2ResistanceRmin;
        }
        else if (position > 256)
        {
            return this->_pot2ResistanceRmax;
        }
        return ((this->_pot2ResistanceRAB / 256) * (float)position) + this->_pot2ResistanceRW;
    }
    else 
    {
        if (position < 0)
        {
            return this->_pot3ResistanceRmin;
        }
        else if (position > 256)
        {
            return this->_pot3ResistanceRmax;
        }
        return ((this->_pot3ResistanceRAB / 256) * (float)position) + this->_pot3ResistanceRW;
    }
}
