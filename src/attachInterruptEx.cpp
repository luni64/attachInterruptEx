#include "CallbackHelper.h"
#include "attachInterruptEx.h"
#include <array>
#include "indexSequences.h"

namespace AttachInterruptEx
{
    using std::array;

    callbackHelper_t callbackHelper; // helps to generate callbacks from various parameters (function pointers, lambdas, functors...)

    template <unsigned nr>
    void relay();

    // generates and preset an array at COMPILETIME (num_slots entries), sets pin and callback fields to default values
    // and presets the relay pointer field to point to a relay<n> function which calls callback[n].
    // (std::array is a very shallow wrapper around c-arrays and does not use any dynamic memory allocation)
    template <std::size_t... numbers>
    constexpr std::array<callbackInfo, num_slots>
    create_callbackInfos(index_sequence<numbers...>)
    {
        return {callbackInfo{-1, nullptr, relay<numbers>}...};
    }
    array<callbackInfo, num_slots> callbackInfos{create_callbackInfos(make_index_sequence<num_slots>{})};

    // the void(*)() relay<n> functions will be attached to the stock attachInterrupt function
    // each relay<n>() will invoke the corresponding callback stored in callbackInfos[n] .
    // the required relay<n> functions are generated while presetting the callbackInfos array at compile time.
    template <unsigned nr>
    void relay()
    {
        callbackInfos[nr].callback->invoke();
    }



}
