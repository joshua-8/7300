#ifndef TMC7300_H
#define TMC7300_H
#include "TMC7300_Map.h"
#include "TMCSerial.h"
#include <Arduino.h>

#define TMC_WRITE_BIT 0x80

/**
 * @brief  Top level class for the TMC7300 library. This class controls a TMC7300 chip.
 */
class TMC7300IC {
public:
    /**
     * @brief  constructor for class that controls a TMC7300 chip.
     * @param  _pin: pin to send uart signals through
     * @param  _chipAddress: The address is selected by AD0 (bit 0, LSB) and AD1 (bit 1, MSB) in the range 0 to 3
     * @param  _baudrate: approximate bits per second to communicate with, default 100000
     */
    TMC7300IC(uint8_t _pin, uint8_t _chipAddress, uint32_t _baudrate = 100000)
        : pin(_pin)
        , chipAddress(_chipAddress)
    {
        // TODO: limit to TMC7300's allowable speeds
        uartDelay = (1000000U / _baudrate);
    }
    void begin(boolean extcap = true)
    {
        pinMode(pin, OUTPUT);
        digitalWrite(pin, HIGH);
        delay(10); // TODO: WHAT DELAY ON STARTUP IS NEEDED?
        writeField(TMC7300_PWM_DIRECT, 1);
        writeField(TMC7300_EXTCAP, extcap); // capacitor on vcp
    }

protected:
    uint8_t pin;
    uint8_t chipAddress;
    uint32_t uartDelay;

    // clang-format off
    int32_t registers[10] = {
//	0	1	2	3	4	5	6	7	8	9
	0,	0,	0,	0,	0,	0,	0,	0,	0,	0};
    // clang-format on

public:
    void writeField(TMCField field, uint32_t fieldValue, boolean write = true)
    {
        uint32_t registerValueToSend = field.setField(fieldValue, registers[field.valueAddress()]);
        if (write) {
            writeRegister(field.address(), registerValueToSend);
        }
        registers[field.valueAddress()] = registerValueToSend;
    }
    uint32_t readField(TMCField field, boolean read = true)
    {
        // uint32_t registerValueReceived = readRegister(field.address());
        // registers[field.valueAddress()] = registerValueReceived;
        return field.getField(registers[field.valueAddress()]);
    }
    void writeRegister(uint8_t address, uint32_t value)
    {

        // from https://github.com/bread-wolf/TMCSerial/blob/cb87877ab4f3c39ed7a5b1ec99cf73eb0f4d5a6c/TMCSerial.cpp#L78C4-L94C6
        uint8_t addressWrite = address | TMC_WRITE_BIT;
        uint8_t writeRequest[8] = { SYNC_BYTE, chipAddress, addressWrite, 0, 0, 0, 0, 0 };

        /* Fill data to be written */
        writeRequest[3] = (value >> 24) & 0xFF;
        writeRequest[4] = (value >> 16) & 0xFF;
        writeRequest[5] = (value >> 8) & 0xFF;
        writeRequest[6] = (value) & 0xFF;

        /* Calculate CRC */
        writeRequest[7] = calcCRC(writeRequest, 7);

        delayMicroseconds(uartDelay);
        // TODO: use hardware specific delays with more precision
        /* Write data */
        for (uint8_t j = 0; j < 8; j++) {
            digitalWrite(pin, LOW); // Start bit
            delayMicroseconds(uartDelay);
            for (uint8_t i = 0; i < 8; i++) {
                digitalWrite(pin, (writeRequest[j] >> i) & 0x01);
                delayMicroseconds(uartDelay);
            }
            digitalWrite(pin, HIGH); // Stop bit
            delayMicroseconds(uartDelay);
        }
        delayMicroseconds(uartDelay * 66);
    }
    // uint32_t readRegister(uint8_t address)
    // {
    //     return 0;
    // }
};
#endif // TMC7300_H
