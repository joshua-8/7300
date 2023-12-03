#ifndef TMC_FIELD_H
#define TMC_FIELD_H

#include <Arduino.h>

// modified from https://github.com/bread-wolf/TMCField/blob/8f3a6262acd8b2a699c473451a84f5d03436e87d/TMCField.hpp#L13C1-L41C3
class TMCField {
public:
    /* Class constructor, initializes private attributes. */
    TMCField(uint32_t _address, uint32_t _mask, uint32_t _shift, uint32_t _valueAddress)
        : mask(_mask)
        , addr(_address)
        , shift(_shift)
        , valueAddr(_valueAddress)
    {
    }

    /* Sets fieldValue inside registerValue. */
    inline uint32_t setField(uint32_t fieldValue, uint32_t registerValue)
    {
        return (registerValue & (~mask)) | ((fieldValue << shift) & mask);
    }

    /* Extracts the value of the field from registerValue. */
    inline uint32_t getField(uint32_t registerValue)
    {
        return (registerValue & mask) >> shift;
    }

    /* Gets address of register that contains this field. */
    inline uint32_t address()
    {
        return addr;
    }
    inline uint32_t valueAddress()
    {
        return valueAddr;
    }

private:
    const uint32_t mask, addr;
    const uint8_t shift;
    const uint32_t valueAddr;
};

#endif // TMC_FIELD_H
