# Savant

some notes on what I'm trying to achieve here.

this is really a learning project - i want to better understand JSI internals and flex my C++ chops. react native's [JSI](https://github.com/facebook/react-native/tree/main/packages/react-native/ReactCommon/jsi) gives us direct access to the JavaScript engine, which means we can call C++ functions from JS with minimal overhead. but honestly, i'm diving in to see how it all works under the hood.

right now i have the basic structure in place - the `install` function that will register our native modules with the JSI `Runtime`. i'm planning to add math operations (hence `math_ops.cpp`), but mostly as an excuse to explore how to expose functions, handle types, work with the JSI API. do i expose individual functions? create a namespace? register methods on a global object? i'm figuring it out as i go.

the CMake setup pulls in react native's JSI headers from the `third_party` submodule. i'm using C++20 and making sure everything is position-independent for static linking. getting the build system right has been its own learning experience.

this is really about digging into the mechanics - understanding how JSI's [`Runtime`](https://github.com/facebook/react-native/blob/main/packages/react-native/ReactCommon/jsi/jsi/jsi.h) works, how `Value` and `Object` interact, what `HostObject`s are all about, and writing some solid C++ along the way.

## useful links

- [JSI header file](https://github.com/facebook/react-native/blob/main/packages/react-native/ReactCommon/jsi/jsi/jsi.h) - the main JSI API reference
- [React Native JSI directory](https://github.com/facebook/react-native/tree/main/packages/react-native/ReactCommon/jsi) - JSI source code
