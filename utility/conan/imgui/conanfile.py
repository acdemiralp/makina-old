from   conans       import ConanFile, CMake, tools
from   conans.tools import download, unzip
import os
import shutil

class ImguiConan(ConanFile):
    name            = "imgui"
    version         = "1.53"     
    description     = "Conan package for imgui."           
    url             = "https://github.com/ocornut/imgui"
    license         = "MIT"                                         
    settings        = "arch", "build_type", "compiler", "os"
    generators      = "cmake"
    options         = {"shared": [True, False]} 
    default_options = "shared=False"

    def source(self):
        zip_name = "v%s.zip" % self.version
        download   ("%s/archive/%s" % (self.url, zip_name), zip_name, verify=False)
        unzip      (zip_name)
        os.unlink  (zip_name)
        download   ("https://github.com/acdemiralp/imgui-cmake/archive/1.0.0.zip", zip_name, verify=False)
        unzip      (zip_name)
        os.unlink  (zip_name)
        shutil.move("imgui-cmake-1.0.0/CMakeLists.txt", ("%s-%s" % (self.name, self.version)))

    def build(self):
        cmake          = CMake(self)
        shared_options = "-DBUILD_SHARED_LIBS=ON" if self.options.shared else "-DBUILD_SHARED_LIBS=OFF"
        self.run("cmake %s-%s %s %s" % (self.name, self.version, cmake.command_line, shared_options))
        self.run("cmake --build . %s" % cmake.build_config)

    def package(self):
        include_folder = "%s-%s" % (self.name, self.version)
        self.copy("*.h"  , dst="include", src=include_folder)
        self.copy("*.hpp", dst="include", src=include_folder)
        self.copy("*.inl", dst="include", src=include_folder)
        self.copy("*.a"  , dst="lib", keep_path=False)
        self.copy("*.so" , dst="lib", keep_path=False)
        self.copy("*.lib", dst="lib", keep_path=False)
        self.copy("*.dll", dst="bin", keep_path=False)

    def package_info(self):
        self.cpp_info.libs = [self.name]