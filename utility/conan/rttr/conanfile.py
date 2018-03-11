from   conans       import ConanFile, CMake, tools
from   conans.tools import download, unzip
import os

class RttrConan(ConanFile):
    name            = "rttr"
    version         = "0.9.5"     
    description     = "Conan package for rttr."           
    url             = "https://github.com/rttrorg/rttr"
    license         = "MIT"                                         
    settings        = "arch", "build_type", "compiler", "os"
    generators      = "cmake"
    options         = {"shared": [True, False], "rtti": [True, False]} 
    default_options = "shared=True", "rtti=True"

    def source(self):
        zip_name = "v%s.zip" % self.version
        download ("%s/archive/%s" % (self.url, zip_name), zip_name, verify=False)
        unzip    (zip_name)
        os.unlink(zip_name)

    def build(self):
        cmake          = CMake(self)
        shared_options = "-DBUILD_STATIC=OFF" if self.options.shared else "-DBUILD_STATIC=ON"
        rtti_options   = "-DBUILD_WITH_RTTI=ON" if self.options.rtti else "-DBUILD_WITH_RTTI=OFF"
        self.run("cmake %s-%s %s %s %s" % (self.name, self.version, cmake.command_line, shared_options, rtti_options))
        self.run("cmake --build . %s" % cmake.build_config)

    def package(self):
        include_folder = "%s-%s/src" % (self.name, self.version)
        self.copy("*.h"  , dst="include", src=include_folder)
        self.copy("*.hpp", dst="include", src=include_folder)
        self.copy("*.inl", dst="include", src=include_folder)
        self.copy("*.a"  , dst="lib", keep_path=False)
        self.copy("*.so" , dst="lib", keep_path=False)
        self.copy("*.lib", dst="lib", keep_path=False)
        self.copy("*.dll", dst="bin", keep_path=False)

    def package_info(self):
        self.cpp_info.libs = ["rttr_core_d"] if self.settings.build_type == "Debug" else ["rttr_core"]
