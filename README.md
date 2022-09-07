# attachInterruptEx

You can use attachInterruptEx in exactly the same way as you use the standard attachInterrupt function. However, it accepts more or less anything which can be called (functions, member functions, lambdas, functors) as callbacks.

The library uses the CallbackHelper library (https://github.com/luni64/CallbackHelper) to handle the various callback types. It has a very small memory footprint and doesn't use dynamic memory allocation. Works nicely even on a Teensy LC.

## Examples

The following example shows how to use a capturing lambda expression to attach a callback to the pin interrupts 0..12. The attached la simply printing the pin nu
```c++
#include "attachInterruptEx.h"

void onPinInterrupt(unsigned pinNr) // will be called on any interrupt from pin 0 to pin 12.
{
    Serial.println(pinNr);
}

void setup()
{
    for (int pin = 0; pin < 13; pin++)
    {
        pinMode(pin, INPUT_PULLUP);
        attachInterruptEx(pin, [pin] { onPinInterrupt(pin); }, FALLING); // capture a copy of pin. The lambda calls onPinInterrupt with the captured pin.
    }
}

void loop(){}
```

The next example shows how to encapsulate and setup a pin interrupt callback function in a user class. The EdgeCounter class simply counts all edges (rising/falling) it sees on a digital pin. The pin to be monitored is settable with the begin() function.

```c++
#include "attachInterruptEx.h"

class EdgeCounter
{
 public:
    void begin(unsigned pin)
    {
        counter = 0;
        pinMode(pin, INPUT_PULLUP);
        attachInterruptEx(pin, [this] { this->ISR(); }, CHANGE);
    }

    unsigned getCounter() { return counter; }

 protected:
    void ISR(){
        counter++;
    }

    unsigned counter;
};

//-------------------------

EdgeCounter ec1, ec2;

void setup()
{
    ec1.begin(0); // counts edges on pin 0
    ec2.begin(1); // counts edges on pin 1
}

void loop()
{
    Serial.printf("Detected edges: Pin1: %u Pin2:%u\n", ec1.getCounter(), ec2.getCounter());
    delay(250);
}
```