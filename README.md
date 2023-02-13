# About
This is the refactored version CPSC 591 term project. The directories are named so because it was initially meant to render procedurally generated clouds. 

This project utilizes the [`Metal-cpp`](https://developer.apple.com/metal/cpp/) C++ wrapper, which requires macOS SDK newer than 12.3. Some Xcode project settings regarding code signingmight need tweaking in order to build it successfully.

# Structure
Rendering passes are refactored into classes under `CloudRendering/Passes/`. `RenderLoopManager` manages the cycle of rendering and GPU resources used by different passes. The `RendererView` ObjC class initializes the `RenderLoopManager` and displays the swap chain (in Metal's term, `CAMetalDrawable` objects). All shaders are in `CloudRendering/Shaders/Shaders.metal`.
