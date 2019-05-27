# Gesture Recognition API Client
This allows the integration of gesture input to any C++ application code

Bindings could be made for other languages. However, this library is natively in C++.

# How To Use

0. Download the latest version of the [gesture API server](https://github.com/bronozoj/GestureAPIServer) and have it running.
1. Grab the files `gestureapi.cc` and `gestureapi.hpp` from the [Github link](https://github.com/bronozoj/GestureAPISample).
2. Include the files to your project and add `#include "gestureapi.hpp"` to the top of every code that will use the API.
3. Make an instance of the `gestureapi::GestureInput` class and call the `begin()` method.
4. Poll for input by calling the `input()` method and feeding the result to methods like `isLeft()` or `isClockwise()` to check for presence of the gesture
5. Before program exit, call the `end()` method to clean up the API instance.

For a working sample, please check the sample applciation in the [Github page](https://github.com/bronozoj/GestureAPISample).