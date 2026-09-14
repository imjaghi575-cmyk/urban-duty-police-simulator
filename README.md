# Urban Duty — Native Police Simulator

**Urban Duty** is a custom-built police simulation for **Windows PC and Android**, focused on realistic driving, police procedures, traffic, pedestrians, investigations, pursuits, weather, and a physically grounded world.

## Project principles

- **Custom native engine** — no Unity, Unreal, or Godot runtime.
- **C++20 + CMake** shared engine core.
- **Vulkan renderer** as the high-fidelity graphics backend.
- Windows and Android targets from the same engine architecture.
- Deterministic/fixed-step simulation foundations for physics and gameplay.
- Scalable rendering and content budgets for desktop and mobile hardware.
- Tests and CI are part of the project, not an afterthought.

## Planned gameplay

- Police patrols and traffic stops
- Driver/document checks and citations
- Arrests, crimes, investigations and crime scenes
- Random events and story missions
- Pursuits and emergency response
- Police vehicles, lights and sirens
- Traffic and pedestrian simulation
- Day/night cycle and weather
- Vehicle damage and physically grounded driving
- Equipment, radio, progression and save/load
- PC keyboard/mouse/controller and Android touch controls

## Engine roadmap

1. Platform/runtime and build system
2. Vulkan device/context/surface/swapchain layer
3. Math, memory and resource systems
4. Fixed-step physics and vehicle dynamics
5. PBR/HDR renderer, shadows, temporal techniques and streaming
6. Asset cooking and shader pipeline
7. World streaming and simulation
8. Input, UI, menus, settings and accessibility
9. AI, traffic, pedestrians and police systems
10. Gameplay vertical slice
11. Android APK and Windows release packaging
12. Profiling, regression testing and content expansion

## Build status

This repository is under active development. The current branch contains the renderer foundation and automated build configuration; a production APK is **not claimed until the Android packaging pipeline produces and verifies an APK artifact**.

## Quality target

The goal is not to maximize numbers blindly. Physics, rendering, AI and streaming are designed around measurable constraints, stability, determinism where appropriate, hardware capability detection, and platform-specific performance budgets.

## Search / project identity

Canonical project name: **Urban Duty — Native Police Simulator**  
Repository: `imjaghi575-cmyk/urban-duty-police-simulator`

For development notes and architecture decisions, see `Docs/ARCHITECTURE.md`.
