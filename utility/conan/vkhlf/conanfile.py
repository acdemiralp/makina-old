from   conans       import ConanFile, CMake, tools
from   conans.tools import download, unzip
import os

class VkhlfConan(ConanFile):
    name            = "vkhlf"
    version         = "master"     
    description     = "Conan package for VkHLF."           
    url             = "https://github.com/nvpro-pipeline/VkHLF"
    license         = "MIT"                                         
    settings        = "arch", "build_type", "compiler", "os"
    generators      = "cmake"
    options         = {"shared": [True, False]} 
    default_options = "shared=True"

    def source(self):
        self.run("git clone --recurse-submodules %s %s-%s" % (self.url, self.name, self.version))

    def build(self):
        cmake          = CMake(self)
        shared_options = "-DBUILD_SHARED_LIBS=ON" if self.options.shared else "-DBUILD_SHARED_LIBS=OFF"
        fixed_options  = "-DBUILD_VKCPP_SAMPLES=OFF"
        self.run("cmake %s-%s %s %s %s" % (self.name, self.version, cmake.command_line, shared_options, fixed_options))
        self.run("cmake --build . %s" % cmake.build_config)

    def package(self):
        include_folder = "%s-%s/%s" % (self.name, self.version, self.name)
        self.copy("*.h"  , dst="include", src=include_folder)
        self.copy("*.hpp", dst="include", src=include_folder)
        self.copy("*.inl", dst="include", src=include_folder)
        self.copy("*.a"  , dst="lib", keep_path=False)
        self.copy("*.so" , dst="lib", keep_path=False)
        self.copy("*.lib", dst="lib", keep_path=False)
        self.copy("*.dll", dst="bin", keep_path=False)

    def package_info(self):
        self.cpp_info.libs = [self.name]