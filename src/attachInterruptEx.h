#pragma once

extern void attachInterruptEx(unsigned pin, void(*callback)(),  int mode); // pointer to free function or static member function

// template <typename T>
// extern void attachInterruptEx(unsigned pin, T callback, int mode); // lambas, functors etc


