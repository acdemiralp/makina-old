## Makina NGE ##

### Requirements ###
- Git
- CMake 3.2+
- Conan 1.1+
- A C++14 Compiler

### Building ###
- Create an account at https://bintray.com and note your API key located at https://bintray.com/profile/edit .
- Run CMake and enter your Bintray username and API key when asked.
- Configure and generate. Dependencies will be downloaded, built and linked automatically via Conan through Bintray.

### Profiling ###
- We recommend MTuner for profiling: https://github.com/milostosic/MTuner/releases/download/v4.0.9/mtuner64.zip .

### Publishing new packages ###
- You can publish new packages to the Makina conan repository via running the following in their conanfile directory.
conan export . acdemiralp/makina
conan upload <PACKAGE>/<VERSION>@acdemiralp/makina --all -r=makina