# Urban Duty: Police Simulator — Custom Engine Architecture

## Vision

Urban Duty is a high-fidelity police simulation for Windows and Android. The project does not use Godot, Unreal, Unity, or another game engine. The engine, runtime abstractions, renderer integration, physics model, world systems, gameplay systems, UI framework, save system, and tools are developed as project-owned code.

The project aims for realistic behavior rather than superficial feature count. Graphics and physics are designed around physically meaningful data, scalable execution, profiling, and testable systems.

## Technology Boundary

- **Language:** C++
- **Native build:** CMake
- **Graphics API:** Vulkan
- **Platform integration:** SDL3 only as a low-level library for windowing, input, lifecycle, and selected audio/platform services
- **Android packaging:** Gradle + Android NDK around the shared CMake engine
- **Windows:** native CMake executable/package
- **CI:** GitHub Actions

SDL3 is not the engine. No SDL type may cross into gameplay-facing interfaces.

## Module Graph

```text
Game
  -> Engine/UI, Engine/World, Engine/AI, Engine/Physics, Engine/Audio, Engine/Input
Engine systems
  -> Engine/Core
  -> Engine/Platform abstractions
Platform implementations
  -> SDL3 / OS / Vulkan platform APIs
Renderer
  -> Vulkan
Physics
  -> project-owned simulation and collision code
Assets
  -> project-owned asset formats/cooking/runtime loading
```

Gameplay must not depend directly on Android APIs, Win32 APIs, SDL headers, or Vulkan handles.

## Engine Layers

### Core

Memory utilities, containers where justified, logging, diagnostics, time, UUIDs, configuration, serialization primitives, math, jobs, and service lifetime.

### Platform

Application lifecycle, windows/surfaces, filesystem roots, input devices, threading primitives, high-resolution timing, and platform capabilities.

### Renderer

Device selection, GPU resources, descriptor management, shader/pipeline management, render graph, visibility, materials, lighting, shadows, reflections, post-processing, frame pacing, profiling, and quality tiers.

The renderer is data-driven so the same world can scale from high-end PC settings to Android settings without separate gameplay implementations.

### Physics

Fixed-step simulation, rigid bodies, collision detection, contact constraints, friction, joints, character movement, vehicle dynamics, and damage. SI-like units and explicit coordinate conventions are required.

Vehicle simulation will model, where applicable: mass, center of mass, inertia, engine torque, gearbox, clutch/automatic logic, differential, wheel angular velocity, suspension travel, spring/damper behavior, tire friction/slip, steering geometry, braking, ABS/traction-control behavior, load transfer, surface-dependent grip, collision impulses, and damage state.

### World

Entities, transforms, spatial partitioning, streaming, terrain, roads, buildings, interiors, vegetation, weather volumes, navigation, and world persistence.

### AI

Perception, decision making, navigation, traffic behavior, pedestrian behavior, police behavior, criminal behavior, emergency response, and crowd scheduling.

### Audio

3D emitters, buses, mixing, environmental effects, vehicle sounds, sirens, radio, voice, UI feedback, and dynamic audio priorities.

### UI

Project-owned UI abstraction and widgets for menus, HUD, settings, tutorial/help, accessibility, controls, save/load, mission information, and notifications.

### AssetSystem

Runtime asset registry, versioned binary formats, dependency tracking, streaming, compression, memory budgets, LOD selection, and cooking tools. Source assets are not duplicated into packages without a reason.

## Graphics Fidelity Strategy

High-end graphics are built from real systems, not decorative options. Planned systems include:

- Physically based materials
- HDR rendering
- Directional/point/spot lighting
- Cascaded and/or virtualized shadows as hardware permits
- Screen-space and hardware-supported reflections where available
- Ambient occlusion
- Temporal anti-aliasing or an appropriate platform-specific alternative
- Volumetric atmosphere/fog where affordable
- Water and glass materials
- Weather-driven lighting and surface response
- Terrain and foliage LOD
- Geometry streaming
- Texture streaming
- GPU-driven visibility where beneficial
- Post-processing with explicit quality costs
- Dynamic resolution on constrained hardware

Every graphics setting must connect to a renderer/asset/world budget and expose measurable performance impact.

## Physics Fidelity Strategy

Physics uses a fixed simulation timestep. Rendering interpolates between simulation states. Physics time must not be tied to variable render FPS.

Vehicle and collision behavior must be validated with automated regression tests for stability, conservation/invariants where applicable, expected braking/traction behavior, and edge cases.

"Realistic" means physically plausible and stable, not numerically maximal at all costs. Models will be calibrated against known behavior and profiled so complexity can scale on Android.

## Input Strategy

Gameplay uses actions rather than physical keys/buttons. Backends map keyboard/mouse, controller, and Android touch controls to actions. Touch layouts are configurable and can be adapted to screen aspect ratio and safe areas.

## Settings Strategy

Settings are registered with metadata and a runtime binding. A setting that has no implementation is not presented as working.

Planned categories include:

- Display
- Graphics
- Performance
- Quality presets
- Rendering features
- Camera
- Audio
- Controls
- Touch
- Controller
- Gameplay
- Vehicle assists
- AI/crowd density
- Traffic density
- HUD
- Accessibility
- Language
- Notifications
- Save/data

Hardware capability detection selects a safe initial preset. Users can override it.

## Save/Data Strategy

Save files are versioned. Schema migrations are explicit. Corrupt or incompatible saves fail safely with diagnostic information instead of silently overwriting data.

## Platform Strategy

### Windows

Primary high-fidelity development target. Development builds enable diagnostics and validation. Release builds disable expensive debug instrumentation while retaining crash/error reporting hooks.

### Android

Arm64 is the primary target. Vulkan capability is queried at runtime. Rendering and simulation budgets adapt to device capabilities. Unsupported features degrade gracefully rather than preventing startup when a lower-quality path exists.

## Performance Budgets

Budgets are measured, not assumed. Each subsystem records CPU/GPU/memory use. Android uses configurable frame-time and memory budgets; Windows exposes higher-quality presets where hardware permits.

## Content and 5 GB Target

A final package near 5 GB is a target budget, not a requirement to fill unused space. Content size should come from useful high-quality meshes, textures, audio, animation, world data, and cooked assets. Compression, streaming, LODs, and platform-specific asset variants are preferred to duplicated content.

## Vertical Slice

The first milestone is intentionally small:

1. Application boot
2. Main menu
3. Settings screen with real bindings
4. New Game
5. Small test environment
6. Controllable character
7. One physically simulated police vehicle
8. Basic camera
9. Save/load
10. Clean exit

Only after this slice builds and runs on the target platforms do we expand into the full city, traffic, AI, crimes, investigations, pursuits, weather, advanced rendering, and content production.
