#include <Arduino.h>
#include <TMC7300.h>

/*
 * Example for https://github.com/joshua-8/TMC7300
 * Connect the uart pin of a TMC7300 to pin 5, and the EN pin of the TMC7300 to pin A1 of the microcontroller, and pull A0 and A1 of the TMC7300 high, and connect two motors to the TMC7300.
 * TMC7300 datasheet: https://www.analog.com/media/en/technical-documentation/data-sheets/TMC7300_datasheet_rev1.08.pdf
 */

const byte ENpin = A1; // EN pin of driver, pulling it low until setup is complete prevents a short twitch on start from motor A
TMC7300IC driver = TMC7300IC(5, 3, 100000); // uartPin, chipAddress, baudrate

void setup()
{
    pinMode(ENpin, OUTPUT);
    digitalWrite(ENpin, LOW); // prevent short
    driver.begin();
    // driver.writeField(TMC7300_IRUN, 10); // current limit for both motor channels

    digitalWrite(ENpin, HIGH);
}

void loop()
{
    driver.writeField(TMC7300_PWM_A, -230, false); // motor a backwards, queue to send with next writeField to either PWMA or PWMB since they share a register
    delay(1000);
    driver.writeField(TMC7300_PWM_B, 150); // motor b forwards
    delay(1000);
    driver.writeField(TMC7300_PWM_A, 250); // motor a forwards
    delay(1000);
    driver.writeField(TMC7300_PWM_B, -230); // motor b backwards
    delay(1000);
}
