#pragma once

#include "attachInterruptEx.h"
#include "callbackHelper.h"
#include "core_pins.h"
#include <array>

constexpr unsigned num_pins = CORE_NUM_DIGITAL;

// setup the callbackHelper
using callbackHelper_t = CallbackHelper<void(void), num_pins>; // callback helper using slots for all digital pins, handles void(void) callbacks
using callback_t       = callbackHelper_t::callback_t;         // type of the callbacks

callbackHelper_t cbh;     // helps to generate callbacks from various parameters (function pointers, lambdas, functors...)
callback_t* cb[num_pins]; // array to store pointers to the generated callbacks

//===============================================================================
// Let the compiler prepare an array of void(*)() relay_functions. These relay
// functions will be attached to the pin interrupts.
// Each relay will invoke its corresponding callback, which is stored
// in the callbacks array

// plain array to store pointers to the actual callbacks
callback_t* callbacks[num_pins];

// relay function to be attached to the pin interrupt
template <unsigned nr>
void relay()
{
    callbacks[nr]->invoke();
}

// helper function to generate a std::array initalized with the corresponding relay functions
template <std::size_t... I>
constexpr std::array<void (*)(), num_pins> MakeRelays(std::index_sequence<I...>)
{
    return std::array<void (*)(), num_pins>{relay<I>...};
}

// Let the compiler generate the array of relay functions (compile-time generation)
constexpr auto relays = MakeRelays(std::make_index_sequence<num_pins>{});

//===================================================================================
//   attachInterruptEx relays the real work and housekeeping to the standard
//   attachInterrupt function//

template <typename T>
void attachInterruptEx(unsigned pin, T&& callback, int mode)
{
    if (pin >= num_pins) return;
    callbacks[pin] = cbh.makeCallback(std::forward<T>(callback), pin); // store the callback function in its array
    attachInterrupt(pin, relays[pin], mode);                           // attach the relay function to the pin interrupt
}
