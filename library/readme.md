## Makina NGE ##

### Requirements ###
- A C++14 Compiler
- CMake 3.11+ https://cmake.org/download
- Conan 1.1+  https://conan.io
- Git         https://git-scm.com/downloads

### Manual Dependencies ###
- FMOD     https://fmod.com/download
- Cuda     https://developer.nvidia.com/cuda-downloads
- OptiX    https://developer.nvidia.com/designworks/optix/download
- Vulkan   https://vulkan.lunarg.com/

### Building ###
- Create an account at https://bintray.com and note your API key located at https://bintray.com/profile/edit .
- Run CMake and enter your Bintray username and API key when asked.
- Configure and generate. The majority of the dependencies will be downloaded, built and linked automatically via Conan through Bintray.

### Profiling & Debugging ###
- MTuner is recommended for profiling: https://github.com/milostosic/MTuner/releases/download/v4.0.9/mtuner64.zip .
- Renderdoc is recommended for GPU debugging: https://renderdoc.org .

### Publishing new packages ###
- You can publish new packages to the Makina conan repository via running the following in their conanfile directory:
```
conan export . acdemiralp/makina
conan upload <PACKAGE>/<VERSION>@acdemiralp/makina --all -r=makina
```