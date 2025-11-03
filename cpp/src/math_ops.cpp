#include <jsi/jsi.h>
#include <cstddef>
#include <utility>
#include "savant/savant.h"

using namespace facebook;

namespace {
  // We do some sanity checking on the arguments to our functions
  void ensureNumberArgs(jsi::Runtime& runtime, const jsi::Value* args, size_t count) {
    if (count < 2 || !args[0].isNumber() || !args[1].isNumber()) {
      throw jsi::JSError(runtime, "savant.add expects two numbers");
    }
  }

  // This is the actual host function implementation that will be called from JS
  // Signature required by createFromHostFunction():
  jsi::Value addHostFunction(jsi::Runtime& runtime, const jsi::Value&, const jsi::Value* args, size_t count) {
    ensureNumberArgs(runtime, args, count);
    double a = args[0].asNumber();
    double b = args[1].asNumber();
    return jsi::Value(a + b);
  }

  jsi::Value subHostFunction(jsi::Runtime& runtime, const jsi::Value&, const jsi::Value* args, size_t count) {
    ensureNumberArgs(runtime, args, count);
    double a = args[0].asNumber();
    double b = args[1].asNumber();
    return jsi::Value(a - b);
  }

}

namespace savant {
  void install(jsi::Runtime& runtime) {
    jsi::PropNameID addName = jsi::PropNameID::forAscii(runtime, "add");
    int paramCount = 2;

    jsi::Function addFunction = jsi::Function::createFromHostFunction(
      runtime,
      addName,
      paramCount,
      [](jsi::Runtime& runtime, const jsi::Value& thisValue, const jsi::Value* args, size_t count) {
        return addHostFunction(runtime, thisValue, args, count);
      });

    jsi::PropNameID subName = jsi::PropNameID::forAscii(runtime, "sub");

    jsi::Function subFunction = jsi::Function::createFromHostFunction(
      runtime,
      subName,
      paramCount,
      [](jsi::Runtime& runtime, const jsi::Value& thisValue, const jsi::Value* args, size_t count) {
        return subHostFunction(runtime, thisValue, args, count);
      });

    jsi::Object global = runtime.global();
    
    jsi::Value existing = global.getProperty(runtime, "savant");
    jsi::Object savantObject = existing.isObject() ? existing.asObject(runtime) : jsi::Object(runtime);

    savantObject.setProperty(runtime, "add", std::move(addFunction));
    savantObject.setProperty(runtime, "sub", std::move(subFunction));
    global.setProperty(runtime, "savant", std::move(savantObject));
  }

}