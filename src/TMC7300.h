#ifndef TMC7300_H
#define TMC7300_H
#include "TMC7300_Map.h"
#include "TMCSerial.h"
#include <Arduino.h>

#define TMC_WRITE_BIT 0x80
// https://www.analog.com/media/en/technical-documentation/data-sheets/TMC7300_datasheet_rev1.08.pdf

/**
 * @brief  Top level class for the TMC7300 library. This class controls a TMC7300 chip.
 */
class TMC7300IC {
public:
    /**
     * @brief  constructor for class that controls a TMC7300 chip.
     * @param  _pin: pin to send uart signals through
     * @param  _chipAddress: The address is selected by AD0 (bit 0, LSB) and AD1 (bit 1, MSB) in the range 0 to 3
     * @param  _baudrate: approximate bits per second to communicate with, default 10000, must be between 10000 and 100000
     */
    TMC7300IC(uint8_t _pin, uint8_t _chipAddress, uint32_t _baudrate = 100000)
        : pin(_pin)
        , chipAddress(_chipAddress)
    {
        _baudrate = constrain(_baudrate, 10000, 100000);
        uartDelay = 1000000 / _baudrate;
    }
    void begin(boolean extcap = true)
    {
        pinMode(pin, OUTPUT);
        digitalWrite(pin, HIGH);
        delay(10);
        writeField(TMC7300_PWM_DIRECT, 1, true);
        writeField(TMC7300_EXTCAP, extcap, true); // capacitor on vcp
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
        uint32_t registerValueReceived = 0;
        if (readRegister(field.address(), registerValueReceived)) {
            registers[field.valueAddress()] = registerValueReceived;
        }
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

        pinMode(pin, OUTPUT);
        digitalWrite(pin, HIGH);
        /* Write data */
        unsigned long m = micros();
        for (int j = 0; j < 8; j++) {
            digitalWrite(pin, LOW); // Start bit
            while (micros() - m < uartDelay) { };
            m += uartDelay;
            for (int i = 0; i < 8; i++) {
                digitalWrite(pin, (writeRequest[j] >> i) & 0x01);
                while (micros() - m < uartDelay) { };
                m += uartDelay;
            }
            digitalWrite(pin, HIGH); // Stop bit
            while (micros() - m < uartDelay) { };
            m += uartDelay;
        }
        delay(1); // prevent messages from being too close together
    }
    boolean readRegister(uint8_t address, uint32_t& value)
    {
        uint8_t addressRead = address & 0b01111111;
        uint8_t readRequest[4] = { SYNC_BYTE, chipAddress, addressRead, 0 };
        readRequest[3] = calcCRC(readRequest, 3);

        uint8_t response[8] = { 0, 0, 0, 0, 0, 0, 0, 0 };

        pinMode(pin, OUTPUT);
        digitalWrite(pin, HIGH);

        uint8_t j = 0;

        unsigned long m = micros();
        /* Write data */
        for (j = 0; j < 4; j++) {
            digitalWrite(pin, LOW); // Start bit
            while (micros() - m < uartDelay) { };
            m += uartDelay;
            for (uint8_t i = 0; i < 8; i++) {
                digitalWrite(pin, (readRequest[j] >> i) & 0x01);
                while (micros() - m < uartDelay) { };
                m += uartDelay;
            }
            digitalWrite(pin, HIGH); // Stop bit
            while (micros() - m < uartDelay) { }
            m += uartDelay;
        }
        pinMode(pin, INPUT_PULLUP);
        while (micros() - m < uartDelay) { };
        delayMicroseconds(20);
        for (j = 0; j < 8; j++) {
            m = micros();
            while (digitalRead(pin) == HIGH && micros() - m < 1000) { } // wait for start bit to start
            m = micros();
            if (micros() - m >= 1000) { // timed out, tmc7300 didn't respond to the read request
                delay(1);
                return false;
            }
            while (micros() - m < uartDelay) { }
            m += uartDelay;
            for (uint8_t i = 0; i < 8; i++) {
                while (micros() - m < uartDelay / 2) { }
                m += uartDelay / 2;
                response[j] |= (digitalRead(pin) << i);
                if (i < 7) {
                    while (micros() - m < uartDelay / 2) { }
                    m += uartDelay / 2;
                } else {
                    while (micros() - m < uartDelay && digitalRead(pin) == LOW) { } // wait for 8th bit to finish
                    delayMicroseconds(1); // make sure the pin has time to go high and stay there
                }
            }
        }
        delay(1); // prevent messages from being too close together
        pinMode(pin, OUTPUT);
        digitalWrite(pin, HIGH);

        if (response[1] != 0xFF) { // "master address"
            return false;
        }
        if (response[2] != addressRead) {
            return false;
        }
        if (response[7] != calcCRC(response, 7)) {
            return false;
        }

        value = (response[3] << 24) | (response[4] << 16) | (response[5] << 8) | response[6];

        return true;
    }
};
#endif // TMC7300_H
