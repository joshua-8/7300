#ifndef TMC7300_H
#define TMC7300_H
#include "TMC7300_Map.h"
#include <Arduino.h>

class TMC7300IC {
protected:
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
        uint32_t registerValueReceived = readRegister(field.address());
        registers[field.valueAddress()] = registerValueReceived;
        return field.getField(registers[field.valueAddress()]);
    }
    void writeRegister(uint8_t address, uint32_t value)
    {
    }
    uint32_t readRegister(uint8_t address)
    {
        return 0;
    }
};
#endif // TMC7300_H
