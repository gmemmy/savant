# subdirectory CMakeLists - no project() declaration needed
# settings are inherited from parent, but we can override/configure here
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

set(RN_THIRD_PARTY "${CMAKE_SOURCE_DIR}/third_party/react-native-src")
set(RN_REACTCOMMON "${RN_THIRD_PARTY}/packages/react-native/ReactCommon")
if(NOT EXISTS "${RN_REACTCOMMON}/jsi/jsi/jsi.h")
  message(FATAL_ERROR "JSI headers not found at ${RN_REACTCOMMON}/jsi/jsi/jsi.h. Did you init the submodule?")
endif()

add_library(savant STATIC
  src/install.cpp
  src/math_ops.cpp
)

target_include_directories(savant PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}/include
  "${RN_REACTCOMMON}"
  "${RN_REACTCOMMON}/jsi"
)