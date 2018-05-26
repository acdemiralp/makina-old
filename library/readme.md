## Makina NGE ##
Approximately 30 minutes to setup, including downloading and installing the dependencies.

### Requirements ###
- A C++17 Compiler ([Clang 5+](https://clang.llvm.org/get_started.html), [GCC 7+](https://gcc.gnu.org) or [MSVC 14.1+](https://www.visualstudio.com/vs))
- [CMake 3.11+](https://cmake.org/download)
- [Conan 1.1+](https://conan.io)
- [Git](https://git-scm.com/downloads)

### Manual Dependencies ###
- [FMOD](https://fmod.com/download)
- [Cuda](https://developer.nvidia.com/cuda-downloads)
- [OptiX](https://developer.nvidia.com/designworks/optix/download)
- [Vulkan](https://vulkan.lunarg.com)

### Building ###
- Download and install the requirements and manual dependencies above (and if asked to whether the installer should add the executable to the system path during any of the installations, choose "yes").
- Create an account at https://bintray.com and note your API key located at https://bintray.com/profile/edit .
- Run CMake and enter your Bintray username and API key when asked.
- Configure and generate. The majority of the dependencies will be downloaded, built and linked automatically via Conan through Bintray.

### Profiling & Debugging ###
- [MTuner](https://github.com/milostosic/MTuner/releases/download/v4.0.9/mtuner64.zip) is recommended for profiling.
- [Renderdoc](https://renderdoc.org) is recommended for GPU debugging.

### Publishing new packages ###
- You can publish new packages to the Makina conan repository via running the following in their conanfile directory:
```
conan export . acdemiralp/makina
conan upload <PACKAGE>/<VERSION>@acdemiralp/makina --all -r=makina
```