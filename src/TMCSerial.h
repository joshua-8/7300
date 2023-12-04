#ifndef TMCSERIAL_H
#define TMCSERIAL_H
#include <Arduino.h>

#define SYNC_BYTE 0x55
#define WRITE_BIT 0x80

// from https://github.com/bread-wolf/TMCSerial/blob/cb87877ab4f3c39ed7a5b1ec99cf73eb0f4d5a6c/TMCSerial.cpp#L181C1-L206C2
uint8_t calcCRC(uint8_t data[], uint8_t dataLength)
{
    uint8_t crc, currentByte;

    /* Initialize crc at 0 */
    crc = 0;
    for (uint8_t j = 0; j < dataLength; j++) {
        currentByte = data[j];
        for (uint8_t k = 0; k < 8; k++) {
            if ((crc >> 7) ^ (currentByte & 0x01)) {
                /* Use 0b100000111 as polynomial */
                crc = (crc << 1) ^ 0x07;
            } else {
                crc = (crc << 1);
            }
            currentByte >>= 1;
        }
    }
    return crc;
}
#endif // TMCSERIAL_H
