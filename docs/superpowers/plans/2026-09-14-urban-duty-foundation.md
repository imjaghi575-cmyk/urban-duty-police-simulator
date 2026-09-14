# Urban Duty Engine Foundation Implementation Plan

> **For agentic workers:** REQUIRED SUB-SKILL: Use superpowers:subagent-driven-development (recommended) or superpowers:executing-plans to implement this plan task-by-task. Steps use checkbox (`- [ ]`) syntax for tracking.

**Goal:** Build the first working foundation of Urban Duty: Police Simulator as a custom C++ engine and game runtime, with no commercial game engine, targeting Windows and Android from one source tree.

**Architecture:** Urban Duty will own its engine/runtime layers: Core, Math, Memory, ECS/world, Renderer, Physics, Audio, Input, UI, AI, AssetSystem, and platform abstraction. Vulkan is the primary renderer; SDL3 is used only as a low-level platform/input/audio/window integration dependency, never as the game engine. CMake owns native builds; Android uses Gradle/NDK around the same native CMake target; GitHub Actions verifies builds.

**Tech Stack:** C++20 or newer supported by the selected CI toolchains; CMake; Vulkan; SDL3; Android NDK/Gradle; Windows native build; GitHub Actions; shader sources compiled to SPIR-V during asset/build processing.

**Spec:** `Docs/ARCHITECTURE.md` (created alongside this plan).

## Global Constraints

- No Godot, Unreal, Unity, or other game engine.
- Engine code and gameplay code are authored by this project.
- SDL3 is an integration/library dependency only; engine abstractions must sit above it.
- Vulkan is the primary high-fidelity rendering backend.
- Windows and Android share the core engine and game code.
- Android support must detect Vulkan capabilities at runtime and degrade quality safely.
- High-fidelity PC rendering and scalable Android rendering must use the same scene/material/asset model.
- Physics must be deterministic where practical and use physically meaningful units, masses, inertia, friction, suspension, braking, drivetrain, collision, and damage models.
- No fake settings: every exposed graphics/physics/gameplay setting must have an implementation owner or be marked unavailable until implemented.
- Early milestones must remain small and buildable; do not inflate repository size with placeholder assets.

---

### Task 1: Repository specification and architecture

**Files:**
- Create: `Docs/ARCHITECTURE.md`
- Create: `Docs/ROADMAP.md`
- Create: `Docs/ENGINE_RULES.md`

**Interfaces:**
- Produces the authoritative module boundaries, dependency rules, platform matrix, quality tiers, and milestone gates used by all later tasks.

- [ ] Write the architecture document covering Core, Platform, Renderer, Physics, Audio, Input, UI, AI, World, AssetSystem, Game, and Tools.
- [ ] Define forbidden dependency directions so gameplay never directly depends on platform-specific APIs.
- [ ] Define Windows/Android build targets and Vulkan capability policy.
- [ ] Define the first vertical slice: boot -> main menu -> new game -> tiny test scene -> controllable player -> save/load -> exit.
- [ ] Define quality tiers and explicit rule that settings must map to real runtime state.
- [ ] Review the documents for contradictions before implementation.

### Task 2: Native build skeleton

**Files:**
- Create: `CMakeLists.txt`
- Create: `Engine/Core/...`
- Create: `Game/...`
- Create: `Platform/Windows/...`
- Create: `Platform/Android/...`
- Create: `Tests/...`
- Create: `.gitignore`
- Create: `CMakePresets.json`

**Interfaces:**
- Produces `UrbanDutyCore` and platform executable/shared-library targets.

- [ ] Add the minimal CMake project and directory structure.
- [ ] Add a Core logging/error/result layer.
- [ ] Add a small platform abstraction for application lifecycle and timing.
- [ ] Add a native test target for core utilities.
- [ ] Build on Linux CI first as the fast host validation.
- [ ] Build Windows target in CI.
- [ ] Build Android arm64 target in CI.

### Task 3: SDL3 platform layer

**Files:**
- Create: `Engine/Platform/...`
- Create/modify: `Platform/Windows/...`
- Create/modify: `Platform/Android/...`
- Create: `ThirdParty/SDL3` integration metadata or pinned dependency configuration.

**Interfaces:**
- `IPlatformWindow`, `IInputBackend`, `IAudioBackend` abstractions consumed by engine systems.

- [ ] Pin a known SDL3 version/commit.
- [ ] Integrate SDL3 through CMake without exposing SDL types to gameplay code.
- [ ] Implement Windows window/event loop.
- [ ] Implement Android lifecycle/window/input bridge.
- [ ] Add input action mapping independent of physical device.

### Task 4: Vulkan renderer foundation

**Files:**
- Create: `Engine/Renderer/Vulkan/...`
- Create: `Shaders/...`
- Create: `Tools/ShaderCompiler/...`

**Interfaces:**
- `IRenderDevice`, `RenderContext`, `GpuBuffer`, `GpuImage`, `Pipeline`, `RenderGraph`.

- [ ] Initialize Vulkan instance/device/swapchain on Windows.
- [ ] Initialize Vulkan surface/device/swapchain on Android.
- [ ] Query supported Vulkan version/features and select a compatible profile.
- [ ] Add validation/debug configuration for development builds.
- [ ] Implement command buffers, synchronization, frame pacing, and resize/orientation handling.
- [ ] Render a minimal test scene.
- [ ] Add shader compilation to SPIR-V.
- [ ] Add render statistics (CPU frame, GPU frame, draw calls, memory counters where available).

### Task 5: Custom math and physics foundation

**Files:**
- Create: `Engine/Physics/...`
- Create: `Engine/Core/Math/...`
- Create: `Tests/Physics/...`

**Interfaces:**
- `RigidBody`, `Collider`, `Material`, `Constraint`, `PhysicsWorld`, `VehicleBody`.

- [ ] Implement unit-safe vectors, quaternions, transforms, matrices, and coordinate conventions.
- [ ] Write failing tests for integration, collision invariants, and coordinate transforms.
- [ ] Implement rigid-body integration and broad/narrow-phase collision foundations.
- [ ] Implement friction/contact constraints.
- [ ] Add suspension, wheel contact, braking, traction, and drivetrain interfaces for vehicles.
- [ ] Add deterministic fixed-step simulation with interpolation for rendering.
- [ ] Add regression tests for physics stability.

### Task 6: Game/runtime vertical slice

**Files:**
- Create: `Game/Police/...`
- Create: `Game/Player/...`
- Create: `Game/Vehicles/...`
- Create: `Game/Missions/...`
- Create: `Engine/UI/...`
- Create: `Engine/Save/...`

**Interfaces:**
- Gameplay consumes engine services only through stable interfaces.

- [ ] Implement boot state and game state machine.
- [ ] Implement main menu: Continue, New Game, Load Game, Settings, Controls, Tutorial, Credits, Exit.
- [ ] Implement a minimal controllable character.
- [ ] Implement one police vehicle using the physics vehicle model.
- [ ] Implement save/load schema with versioning.
- [ ] Implement settings registry with real runtime bindings.
- [ ] Add a tutorial/help screen explaining the current vertical slice.

### Task 7: Android/Windows CI and artifact verification

**Files:**
- Create: `.github/workflows/build.yml`
- Create: `.github/workflows/test.yml`
- Modify: `Docs/ROADMAP.md`

**Interfaces:**
- CI produces verifiable Windows and Android debug/release artifacts for milestones.

- [ ] Build native tests.
- [ ] Build Windows package.
- [ ] Build Android APK for arm64.
- [ ] Upload small milestone artifacts.
- [ ] Record build IDs and checksums in CI logs.
- [ ] Do not claim a build works until CI completes successfully.

### Task 8: Verification gate

**Files:**
- Modify: `Docs/ROADMAP.md`
- Create: `Docs/VERIFICATION.md`

- [ ] Run unit tests.
- [ ] Run static/compiler warnings checks.
- [ ] Verify Vulkan initialization on supported CI/device paths.
- [ ] Verify Android package installs when a device/emulator is available.
- [ ] Verify Windows executable launches.
- [ ] Verify save/load and settings persistence.
- [ ] Document all failures rather than masking them.

---

## Long-Term Fidelity Roadmap

After the foundation is verified, development proceeds in independent vertical slices: city/world streaming; PBR material system; clustered/deferred lighting; shadows and reflections; volumetric atmosphere; animation; crowds; traffic; police/criminal AI; investigation/crime systems; vehicle damage; weather; audio/radio; mission system; accessibility/localization; touch/controller/keyboard input; profiling; asset cooking/compression; and final packaging.

The 5 GB target is a packaging budget, not a quality guarantee. Fidelity comes from authored assets, shaders, materials, simulation, animation, sound, and world density—not padding the package with unused data.
