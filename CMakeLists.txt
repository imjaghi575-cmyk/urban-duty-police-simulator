cmake_minimum_required(VERSION 3.25)
project(UrbanDuty VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

option(URBANDUTY_BUILD_TESTS "Build engine tests" ON)
option(URBANDUTY_FETCH_SDL "Fetch SDL3 when not supplied by the platform" ON)
option(URBANDUTY_ENABLE_VULKAN "Build the native Vulkan renderer layer" ON)

if(ANDROID)
  set(CMAKE_POSITION_INDEPENDENT_CODE ON)
  set(SDL_SHARED OFF CACHE BOOL "" FORCE)
  set(SDL_STATIC ON CACHE BOOL "" FORCE)
  set(SDL_TEST_LIBRARY OFF CACHE BOOL "" FORCE)
  set(SDL_TESTS OFF CACHE BOOL "" FORCE)
  set(SDL_EXAMPLES OFF CACHE BOOL "" FORCE)
  set(SDL_INSTALL OFF CACHE BOOL "" FORCE)
endif()

if(URBANDUTY_FETCH_SDL)
  include(FetchContent)
  FetchContent_Declare(
    SDL3
    GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
    GIT_TAG release-3.4.12
    GIT_SHALLOW TRUE
  )
  set(SDL_TEST_LIBRARY OFF CACHE BOOL "" FORCE)
  set(SDL_TESTS OFF CACHE BOOL "" FORCE)
  set(SDL_EXAMPLES OFF CACHE BOOL "" FORCE)
  FetchContent_MakeAvailable(SDL3)
endif()

if(ANDROID)
  find_library(URBANDUTY_VULKAN_LIBRARY vulkan REQUIRED)
elseif(URBANDUTY_ENABLE_VULKAN)
  find_package(Vulkan QUIET)
endif()

add_library(UrbanDutyCore STATIC
  Engine/Core/Log.cpp
  Engine/Core/Runtime.cpp
)
target_include_directories(UrbanDutyCore PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_compile_features(UrbanDutyCore PUBLIC cxx_std_20)

if(MSVC)
  target_compile_options(UrbanDutyCore PRIVATE /W4 /permissive-)
else()
  target_compile_options(UrbanDutyCore PRIVATE -Wall -Wextra -Wpedantic)
endif()

set(URBANDUTY_HAS_VULKAN OFF)
if(URBANDUTY_ENABLE_VULKAN AND (ANDROID OR Vulkan_FOUND))
  add_library(UrbanDutyVulkan STATIC
    Engine/Renderer/Vulkan/VulkanCapabilities.cpp
    Engine/Renderer/Vulkan/VulkanContext.cpp
  )
  target_include_directories(UrbanDutyVulkan PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
  target_compile_features(UrbanDutyVulkan PUBLIC cxx_std_20)
  target_compile_definitions(UrbanDutyVulkan PRIVATE URBANDUTY_HAS_VULKAN=1)
  if(ANDROID)
    target_link_libraries(UrbanDutyVulkan PUBLIC ${URBANDUTY_VULKAN_LIBRARY})
  else()
    target_link_libraries(UrbanDutyVulkan PUBLIC Vulkan::Vulkan)
  endif()
  if(MSVC)
    target_compile_options(UrbanDutyVulkan PRIVATE /W4 /permissive-)
  else()
    target_compile_options(UrbanDutyVulkan PRIVATE -Wall -Wextra -Wpedantic)
  endif()
  set(URBANDUTY_HAS_VULKAN ON)
endif()

if(ANDROID)
  add_library(UrbanDuty SHARED Platform/SDL3/Main.cpp)
  set_target_properties(UrbanDuty PROPERTIES OUTPUT_NAME main)
else()
  add_executable(UrbanDuty Platform/SDL3/Main.cpp)
endif()

# SDL guarantees the SDL3::SDL3 target. Its implementation can be static or
# shared depending on the options above; do not depend on implementation-
# specific aliases such as SDL3::SDL3-static.
if(TARGET SDL3::SDL3)
  set(URBANDUTY_SDL_TARGET SDL3::SDL3)
else()
  message(FATAL_ERROR "SDL3::SDL3 target was not created")
endif()

target_link_libraries(UrbanDuty PRIVATE UrbanDutyCore ${URBANDUTY_SDL_TARGET})

if(URBANDUTY_HAS_VULKAN)
  target_link_libraries(UrbanDuty PRIVATE UrbanDutyVulkan)
  if(ANDROID)
    target_link_libraries(UrbanDuty PRIVATE ${URBANDUTY_VULKAN_LIBRARY})
  else()
    target_link_libraries(UrbanDuty PRIVATE Vulkan::Vulkan)
  endif()
  target_compile_definitions(UrbanDuty PRIVATE URBANDUTY_HAS_VULKAN=1)
endif()

if(WIN32)
  target_compile_definitions(UrbanDuty PRIVATE URBANDUTY_PLATFORM_WINDOWS=1)
elseif(ANDROID)
  target_compile_definitions(UrbanDuty PRIVATE URBANDUTY_PLATFORM_ANDROID=1)
endif()

if(URBANDUTY_BUILD_TESTS AND NOT ANDROID)
  enable_testing()
  add_executable(UrbanDutyCoreTests Tests/CoreTests.cpp)
  target_link_libraries(UrbanDutyCoreTests PRIVATE UrbanDutyCore)
  add_test(NAME UrbanDutyCoreTests COMMAND UrbanDutyCoreTests)

  if(URBANDUTY_HAS_VULKAN)
    add_executable(UrbanDutyVulkanTests Tests/VulkanCapabilitiesTests.cpp)
    target_link_libraries(UrbanDutyVulkanTests PRIVATE UrbanDutyVulkan)
    add_test(NAME UrbanDutyVulkanTests COMMAND UrbanDutyVulkanTests)
  endif()
endif()
