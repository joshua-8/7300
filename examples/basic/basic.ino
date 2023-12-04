#include <Arduino.h>
#include <TMC7300.h>

/*
 * https://github.com/joshua-8/TMC7300
 */

const byte ENpin = A1; // EN pin of driver, pulling it low until setup is complete prevents a short twitch on start
TMC7300IC driver = TMC7300IC(5, 0, 10000); // uartPin, chipAddress, baudrate

void setup()
{
    pinMode(ENpin, OUTPUT);
    digitalWrite(ENpin, LOW);
    driver.begin();
    digitalWrite(ENpin, HIGH);
}
void loop()
{
    driver.writeField(TMC7300_PWM_A, -230);
    delay(1000);
    driver.writeField(TMC7300_PWM_B, 150);
    delay(1000);
    driver.writeField(TMC7300_PWM_A, 250);
    delay(1000);
    driver.writeField(TMC7300_PWM_B, -230);
    delay(1000);
}
