from   conans       import ConanFile, CMake, tools
from   conans.tools import download, unzip
import os

class MakinaConan(ConanFile):
    name            = "makina"
    version         = "1.0.0"     
    description     = "Conan package for makina."           
    url             = "https://github.com/acdemiralp/makina"
    license         = "MIT"											
    settings        = "arch", "build_type", "compiler", "os"
    generators      = "cmake"
    requires        = (("boost/1.66.0@conan/stable" ),
                       ("di/1.0.0@acdemiralp/makina"),
                       ("ec/1.0.0@acdemiralp/makina"),
                       ("fg/1.0.0@acdemiralp/makina"),
                       ("fi/1.0.0@acdemiralp/makina"),
                       ("gl/1.0.2@acdemiralp/makina"))
    options         = {"shared": [True, False]} 
    default_options = "shared=True"

    def build(self):
        cmake          = CMake(self)
        shared_options = "-DBUILD_SHARED_LIBS=ON" if self.options.shared else "-DBUILD_SHARED_LIBS=OFF"
        self.run("cmake %s-%s %s %s" % (self.name, self.version, cmake.command_line, shared_options))
        self.run("cmake --build . %s" % cmake.build_config)

    def package(self):
        include_folder = "%s-%s/include" % (self.name, self.version)
        self.copy("*.h"  , dst="include", src=include_folder)
        self.copy("*.hpp", dst="include", src=include_folder)
        self.copy("*.inl", dst="include", src=include_folder)
        self.copy("*.a"  , dst="lib", keep_path=False)
        self.copy("*.so" , dst="lib", keep_path=False)
        self.copy("*.lib", dst="lib", keep_path=False)
        self.copy("*.dll", dst="bin", keep_path=False)

    def package_info(self):
        self.cpp_info.libs = [self.name]
        if self.settings.os == "Windows":
            if not self.options.shared:
                self.cpp_info.defines.append("MAKINA_STATIC")