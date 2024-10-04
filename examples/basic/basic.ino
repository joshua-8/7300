#include <Arduino.h>
#include <TMC7300.h>

/*
 * Example for https://github.com/joshua-8/TMC7300
 * Connect the uart pin of a TMC7300 to pin MISO, and the EN pin of the TMC7300 to pin SCK of the microcontroller, and pull A0 and A1 of the TMC7300 low, and connect two motors to the TMC7300.
 * TMC7300 datasheet: https://www.analog.com/media/en/technical-documentation/data-sheets/TMC7300_datasheet_rev1.08.pdf
 *
 * motorA stops when motor b starts (a was set but not written)
 * motorA starts (motorB is still turning)
 * motorB stops (motorA is still turning)
 * longest pause, then restart from the beginning
 */

const byte ENpin = SCK; // EN pin of driver, pulling it low until setup is complete prevents a short twitch on start from motor A
const byte uartPin = MISO; // uart pin of driver
TMC7300IC driver = TMC7300IC(uartPin, 0, 100000); // uartPin, chipAddress, baudrate

void setup()
{
    pinMode(ENpin, OUTPUT);
    digitalWrite(ENpin, LOW);
    driver.begin();
    // driver.writeField(TMC7300_IRUN, 8); // current limit for both motor channels

    digitalWrite(ENpin, HIGH);
    delay(10);
}

void loop()
{
    if(driver.checkDriver())
    {
        Serial.println("Driver settings reset");
        delay(100);
    }

    driver.writeField(TMC7300_PWM_A, 0, false); // motor a stop, queue to send with next writeField to either PWMA or PWMB since they share a register
    delay(1000);
    driver.writeField(TMC7300_PWM_B, 150); // motor b forwards
    delay(1000);
    driver.writeField(TMC7300_PWM_A, 250); // motor a forwards
    delay(1000);
    driver.writeField(TMC7300_PWM_B, 0); // motor b stop
    delay(1000);
}
