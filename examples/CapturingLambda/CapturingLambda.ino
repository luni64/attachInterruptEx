#include "attachInterruptEx.h"

void setup()
{
    for (int pin = 0; pin < 13; pin++)
    {
        pinMode(pin, INPUT_PULLUP);
        attachInterruptEx(pin, [pin] { Serial.println(pin); }, FALLING); // capture a copy of pin
    }
}

void loop()
{
}