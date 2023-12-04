#ifndef TMC7300_H
#define TMC7300_H
#include "TMC7300_Map.h"
#include "TMCSerial.h"
#include <Arduino.h>

#define TMC_WRITE_BIT 0x80

class TMC7300IC {
public:
    TMC7300IC(uint8_t _pin, uint8_t _chipAddress, uint32_t _baudrate = 115200)
        : pin(_pin)
        , chipAddress(_chipAddress)
        , baudrate(_baudrate)
    {
    }
    void begin()
    {
        pinMode(pin, OUTPUT);
        digitalWrite(pin, HIGH);
        delay(10); // TODO: WHAT DELAY ON STARTUP IS NEEDED?
        writeField(TMC7300_PWM_DIRECT, 1);
    }

protected:
    uint8_t pin;
    uint8_t chipAddress;
    uint32_t baudrate;

    // clang-format off
    int32_t registers[10] = {
//	0	1	2	3	4	5	6	7	8	9
	0,	0,	0,	0,	0,	0,	0,	0,	0,	0};
    // clang-format on

public:
    void writeField(TMCField field, uint32_t fieldValue)
    {
        uint32_t registerValueToSend = field.setField(fieldValue, registers[field.valueAddress()]);
        writeRegister(field.address(), registerValueToSend);
        registers[field.valueAddress()] = registerValueToSend;
    }
    uint32_t readField(TMCField field)
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

        unsigned long lastMicros = 0;
        /* Write data */
        for (uint8_t j = 0; j < 8; j++) {
            lastMicros = micros();
            digitalWrite(TX, LOW); // Start bit
            while ((micros() - lastMicros) < (1000000 / baudrate)) { }
            lastMicros = micros();
            for (uint8_t i = 0; i < 8; i++) {
                digitalWrite(TX, (writeRequest[j] >> i) & 0x01);
                while ((micros() - lastMicros) < (1000000 / baudrate)) { }
                lastMicros = micros();
            }
            digitalWrite(TX, HIGH); // Stop bit
            while ((micros() - lastMicros) < (1000000 / baudrate)) { }
            lastMicros = micros();
        }
        // TODO: DELAY SOME BETWEEN MESSAGES?
    }
    // uint32_t readRegister(uint8_t address)
    // {
    //     return 0;
    // }
};
#endif // TMC7300_H
