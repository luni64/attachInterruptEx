#pragma once

#include "Arduino.h"
#include "CallbackHelper.h"
#include <array>

namespace AttachInterruptEx
{
    constexpr unsigned num_slots = 5; // increase if you need more interrupt slots active in parallel

    // setup the callbackHelper
    using callbackHelper_t = CallbackHelper<void(void), num_slots>; // callback helper using num_slots callback slots. Accepts void(void) callbacks (function pointers,lambdas, functors etc)
    using callback_t       = callbackHelper_t::callback_t;          // type of the callbacks
    using relay_t          = void (*)();                            // type of the relay functions which relay the ISR to the actual callback

    struct callbackInfo
    {
        int pin;              // we need the pin number to be able to detach later
        callback_t* callback; // pointer to the actual callback
        relay_t relay;        // relays the ISR to the callback
    };

    extern std::array<callbackInfo, num_slots> callbackInfos; // storage for num_slot callbackInfo structures. 12byte per slot.
    extern callbackHelper_t callbackHelper;                   // generates callbacks from various parameters (function pointers, lambdas, functors...)
}
//=====================================================================================================
// attachInterruptEx relays the real work and housekeeping to the standard attachInterrupt function
// returns false if no free slot was found.
template <typename T>
bool attachInterruptEx(uint_fast8_t pin, T&& callback, int mode)
{
    using namespace AttachInterruptEx;

    for (unsigned slot = 0; slot < num_slots; slot++)
    {
        auto& cbi = callbackInfos[slot]; // save typing
        if (cbi.pin == -1)
        {
            cbi.callback = callbackHelper.makeCallback(std::forward<T>(callback), slot); // get and store a pointer to the actual callback function
            cbi.pin      = pin;                                                          // we need the pin allow detaching
            ::attachInterrupt(cbi.pin, cbi.relay, mode);                                 // cbi.relay is preset at compile time
            return true;
        }
    }
    return false; // didn't find a free slot, ignore request and inform the caller
}

//=====================================================================================================
// detachInterruptEx gets the slot number where the pin interrupt was stored,
// uses the standard detachInterrupt function to detach the pin,
// updates to callbackInfos array (sets back the entry to default values)
inline void detachInterruptEx(int_fast8_t pin)
{
    using namespace AttachInterruptEx;
    for (unsigned slot = 0; slot < num_slots; slot++)
    {
        auto& cbi = callbackInfos[slot];
        if (cbi.pin == pin)
        {
            ::detachInterrupt(pin);
            cbi.pin      = -1;
            cbi.callback = nullptr;
        }
    }
}
