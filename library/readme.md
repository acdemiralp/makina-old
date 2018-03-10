## Makina NGE ##
- Requires cmake (3.2+), conan (1.1+), and a C++14 compiler.
- Dependencies are built and linked automatically.
- Ensure you have an active internet connection on first cmake generation.

## Publishing new dependencies ##
- First, add the makina bintray repository via:
```
conan remote add makina https://api.bintray.com/conan/acdemiralp/makina
conan user -p <APIKEY> -r makina <USERNAME>
```
- You can then export new packages via running the following in their conanfile directory.
conan export . acdemiralp/makina
conan upload <PACKAGE>/<VERSION>@acdemiralp/makina --all -r=makina