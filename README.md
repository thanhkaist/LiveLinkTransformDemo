# LiveLinkTransformDemo

A demo application that streams real-time transform data to Unreal Engine via the [Unreal Live Link C Interface](https://github.com/ue4plugins/UnrealLiveLinkCInterface). The demo moves a subject named `circle` in a circular path on the X-Y plane, illustrating how external C/C++ applications can drive objects inside Unreal Engine using the Live Link protocol.

## Overview

The Unreal Live Link plugin allows external applications to stream animation, transform, camera, and light data directly into Unreal Engine at runtime. This project demonstrates the **Transform** role: it continuously sends position updates for a single subject, making it trace `300` full circles of radius `200` Unreal units at `100` units above the origin.

The application dynamically loads the `UnrealLiveLinkCInterface` shared library (`.dll` on Windows, `.so` on Linux) and communicates with a running Unreal Engine instance over the local network.

## Repository Structure

| File | Description |
|------|-------------|
| `main.cpp` | Demo entry point — loads the interface and sends circular transform frames |
| `UnrealLiveLinkCInterfaceAPI.h` | Public API header (function pointer declarations) |
| `UnrealLiveLinkCInterfaceAPI.c` | Loads the shared library and resolves all function pointers |
| `UnrealLiveLinkCInterfaceTypes.h` | Shared data structures and constants (`Transform`, `Camera`, `Light`, etc.) |
| `UnrealLiveLinkCInterface.h` | Exported symbol declarations for the shared library |
| `UnrealLiveLinkCInterface.dll` | Pre-built Windows shared library (API version 5) |
| `UnrealLiveLinkCInterface.lib` | Import library for the Windows DLL |
| `DemoLiveLink.pro` | Qt project file |

## Prerequisites

- **Qt 5** (or later) with `qmake` — the project uses Qt's `QCoreApplication` and `QDebug`
- **C++11** compatible compiler (MSVC on Windows, GCC/Clang on Linux)
- A running **Unreal Engine** instance with the **Live Link** plugin enabled and configured to accept incoming connections

## Building

### Windows (Qt Creator or command line)

```bat
qmake DemoLiveLink.pro
nmake          # or mingw32-make if using MinGW
```

The build links against the provided `UnrealLiveLinkCInterface.lib`. Copy `UnrealLiveLinkCInterface.dll` to the same directory as the resulting executable before running.

### Linux

```bash
qmake DemoLiveLink.pro
make
```

On Linux the application looks for `UnrealLiveLinkCInterface.so` at runtime. Ensure the library is on your `LD_LIBRARY_PATH` or in the same directory as the executable.

## Usage

1. Open your Unreal Engine project and enable the **Live Link** plugin (*Edit → Plugins → Animation → Live Link*).
2. In the Live Link panel (*Window → Live Link*), add a new **Message Bus Source** so that Unreal listens for incoming connections.
3. Run the compiled demo executable:

   ```bash
   ./DemoLiveLink          # Linux
   DemoLiveLink.exe        # Windows
   ```

4. The application will:
   - Load the `UnrealLiveLinkCInterface` shared library.
   - Register a subject named **`circle`** with the Transform role.
   - Send `100 000` frames (~16 ms apart) that move `circle` in a circular path.
   - Unload the library and exit when done.

5. In Unreal Engine, the `circle` subject will appear in the Live Link panel. You can bind it to any Actor's transform via a **Live Link Controller** component or an Animation Blueprint.

## Demo Parameters

The demo behaviour can be adjusted by editing the `#define` constants at the top of `main.cpp`:

| Constant | Default | Description |
|----------|---------|-------------|
| `CIRCLES` | `300` | Number of complete circles to perform |
| `CIRCLE_RADIUS` | `200` | Radius of the circle in Unreal units (cm) |
| `STEP_COUNT` | `100000` | Total number of frames to send |
| `HEIGHT` | `100` | Z-axis offset above the origin |

## API Overview

The C interface supports five Live Link *roles*. Each role follows the same two-step pattern:

1. **Set structure** — called once to declare the subject and its static properties.
2. **Update frame** — called every frame to push new per-frame data.

| Role | Set structure | Update frame |
|------|--------------|--------------|
| Basic (generic floats) | `UnrealLiveLink_SetBasicStructure` | `UnrealLiveLink_UpdateBasicFrame` |
| Animation (skeleton) | `UnrealLiveLink_SetAnimationStructure` | `UnrealLiveLink_UpdateAnimationFrame` |
| **Transform** | `UnrealLiveLink_SetTransformStructure` | `UnrealLiveLink_UpdateTransformFrame` |
| Camera | `UnrealLiveLink_SetCameraStructure` | `UnrealLiveLink_UpdateCameraFrame` |
| Light | `UnrealLiveLink_SetLightStructure` | `UnrealLiveLink_UpdateLightFrame` |

The current API version is **5** (`UNREAL_LIVE_LINK_API_VERSION`).

## References

- [Unreal Engine Live Link documentation](https://docs.unrealengine.com/en-US/Engine/Animation/LiveLinkPlugin/index.html)
- [MayaLiveLink plugin (ue4plugins)](https://github.com/ue4plugins/MayaLiveLink)
- [Dynamic library loading in C](http://www.cplusplus.com/articles/48TbqMoL)

## License

Copyright © 2020 Patrick Palmer, The Jim Henson Company.

Licensed under the [MIT License](https://opensource.org/licenses/MIT). See the license header in `UnrealLiveLinkCInterfaceAPI.h` for the full text.
