# Savant

some notes on what I'm trying to achieve here.

this is really a learning project - i want to better understand JSI internals and flex my C++ chops. react native's [JSI](https://github.com/facebook/react-native/tree/main/packages/react-native/ReactCommon/jsi) gives us direct access to the JavaScript engine, which means we can call C++ functions from JS with minimal overhead. but honestly, i'm diving in to see how it all works under the hood.

## why JSI?

to understand why JSI exists, you have to understand what came before - the old bridge architecture. the bridge will invoke the JSC API to start some work on the JS runtime and the runtime will batch serialized JSON responses encoding the native modules instructions. then the bridge will deserialize the response, look up the module and invoke its methods.

the main problems with the old approach:

- **separate memory spaces**: JS and native lived in completely different memory spaces. think of it like two people in different rooms trying to talk - they had to write letters (serialize to JSON), pass them through a mail slot (the bridge), and then read them aloud (deserialize) on the other side.
- **slow communication**: every single call had to be serialized → passed over the bridge → deserialized → dispatched. the bridge would batch these serialized messages, but every call still incurred heavy overhead (serialization cost + thread hopping). imagine ordering coffee by writing your order on paper, handing it to a runner, waiting for them to cross the building, then having someone read it and make your coffee. that's the old bridge.
- **tight coupling**: the implementation was tightly coupled with JSC because it relied on its specific C API. there was no way to use another engine without rewriting the framework. it's like building a house that only works with one specific brand of foundation - try to swap it out and everything breaks.
- **engine lock-in**: C is challenging to work with - it's verbose, error prone and you have to manage memory yourself. plus, being locked into JSC meant react native couldn't easily support other engines like V8 or Hermes.

**in short: tight coupling, slow communication, and engine lock-in.**

JSI changes all of that. it's a C++ interface that defines the minimal contract between JS engines and the react native runtime. here's what that means:

- **shared memory space**: because it's written in C++, both JS and native runtimes can share the same memory space - no more JSON serialization. think of it like both people are now in the same room and can just talk directly to each other, or even better, they're both looking at the same whiteboard they can write on simultaneously.
- **direct value manipulation**: you can directly hold JS values (via `jsi::Value`, `jsi::Object`, etc.) and manipulate them natively. instead of sending a description of a car over the bridge, you're both looking at the same actual car.
- **synchronous calls**: functions are called synchronously without crossing a bridge boundary. going back to the coffee analogy - now you can just walk up to the barista and tell them directly what you want, no paper, no runner, no waiting.
- **engine agnostic**: the "bridge" still exists conceptually, but it's no longer a physical serialization channel - it's an abstraction layer over shared memory access. this means you can swap out the underlying JS engine (JSC, V8, Hermes) without rewriting your code.

the flow changed from:

```
Native → serialize JSON → bridge → deserialize → JS
```

to:

```
Native → call JSI → dispatch into shared C++ memory → visible in JS runtime
```

both runtimes have access to the same shared memory. it's pretty elegant when you think about it - like two people sharing a notebook where they can both read and write, instead of constantly photocopying pages and passing them back and forth.

## what i'm building

right now i have the basic structure in place - the `install` function that will register our native modules with the JSI `Runtime`. i'm planning to add math operations (hence `math_ops.cpp`), but mostly as an excuse to explore how to expose functions, handle types, work with the JSI API. do i expose individual functions? create a namespace? register methods on a global object? i'm figuring it out as i go.

getting the build system right has been its own learning experience. the CMake setup pulls in react native's JSI headers from the `third_party` submodule using sparse-checkout - only pulling what we need (`ReactCommon/jsi` and `ReactCommon/callinvoker`). turns out in newer versions of react native, `ReactCommon` lives under `packages/react-native/ReactCommon`, not at the root. that took some digging to figure out.

i'm using C++20 (the spaceship operator `<=>` is pretty neat) and making sure everything is position-independent for static linking. CMake presets make it easy to switch between debug, release, and debug-asan builds. for linting, i'm using clangd since Cursor can't use the Microsoft C/C++ extension - it's actually faster and works better anyway.

## what i'm exploring

this is really about digging into the mechanics - understanding how JSI's [`Runtime`](https://github.com/facebook/react-native/blob/main/packages/react-native/ReactCommon/jsi/jsi/jsi.h) works, how `Value` and `Object` interact, what `HostObject`s are all about, and writing some solid C++ along the way. seeing how these shared memory objects actually work in practice, how synchronous calls feel compared to the old async bridge pattern, and how you structure native modules to take advantage of it all.

## build setup

to build:

```bash
cmake --preset debug-asan -S cpp -B cpp/build/debug-asan
cmake --build cpp/build/debug-asan
```

the `compile_commands.json` gets generated automatically so clangd can provide proper linting and IntelliSense. the whole setup feels pretty solid now.

## useful links

- [JSI header file](https://github.com/facebook/react-native/blob/main/packages/react-native/ReactCommon/jsi/jsi/jsi.h) - the main JSI API reference
- [React Native JSI directory](https://github.com/facebook/react-native/tree/main/packages/react-native/ReactCommon/jsi) - JSI source code
