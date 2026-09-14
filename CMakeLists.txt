cmake_minimum_required(VERSION 3.25)
project(UrbanDuty VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

option(URBANDUTY_BUILD_TESTS "Build engine tests" ON)
option(URBANDUTY_FETCH_SDL "Fetch SDL3 when not supplied by the platform" ON)

if(URBANDUTY_FETCH_SDL)
  include(FetchContent)
  FetchContent_Declare(
    SDL3
    GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
    GIT_TAG release-3.2.22
    GIT_SHALLOW TRUE
  )
  set(SDL_TEST_LIBRARY OFF CACHE BOOL "" FORCE)
  set(SDL_TESTS OFF CACHE BOOL "" FORCE)
  set(SDL_EXAMPLES OFF CACHE BOOL "" FORCE)
  FetchContent_MakeAvailable(SDL3)
endif()

find_package(Vulkan QUIET)

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

if(Vulkan_FOUND)
  add_library(UrbanDutyVulkan STATIC
    Engine/Renderer/Vulkan/VulkanCapabilities.cpp
  )
  target_include_directories(UrbanDutyVulkan PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
  target_link_libraries(UrbanDutyVulkan PUBLIC Vulkan::Vulkan)
  target_compile_features(UrbanDutyVulkan PUBLIC cxx_std_20)
  if(MSVC)
    target_compile_options(UrbanDutyVulkan PRIVATE /W4 /permissive-)
  else()
    target_compile_options(UrbanDutyVulkan PRIVATE -Wall -Wextra -Wpedantic)
  endif()
endif()

add_executable(UrbanDuty
  Platform/SDL3/Main.cpp
)
target_link_libraries(UrbanDuty PRIVATE UrbanDutyCore SDL3::SDL3)

if(Vulkan_FOUND)
  target_link_libraries(UrbanDuty PRIVATE UrbanDutyVulkan Vulkan::Vulkan)
  target_compile_definitions(UrbanDuty PRIVATE URBANDUTY_HAS_VULKAN=1)
endif()

if(WIN32)
  target_compile_definitions(UrbanDuty PRIVATE URBANDUTY_PLATFORM_WINDOWS=1)
elseif(ANDROID)
  target_compile_definitions(UrbanDuty PRIVATE URBANDUTY_PLATFORM_ANDROID=1)
endif()

if(URBANDUTY_BUILD_TESTS)
  enable_testing()
  add_executable(UrbanDutyCoreTests Tests/CoreTests.cpp)
  target_link_libraries(UrbanDutyCoreTests PRIVATE UrbanDutyCore)
  add_test(NAME UrbanDutyCoreTests COMMAND UrbanDutyCoreTests)

  if(Vulkan_FOUND)
    add_executable(UrbanDutyVulkanTests Tests/VulkanCapabilitiesTests.cpp)
    target_link_libraries(UrbanDutyVulkanTests PRIVATE UrbanDutyVulkan)
    add_test(NAME UrbanDutyVulkanTests COMMAND UrbanDutyVulkanTests)
  endif()
endif()
