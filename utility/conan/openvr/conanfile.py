from conans import CMake, ConanFile
from conans.tools import download, unzip
import os

class OpenVRConan(ConanFile):
    name            = "openvr"
    description     = "API and runtime that allows access to VR hardware."
    version         = "1.0.14"
    url             = "https://github.com/ValveSoftware/openvr"
    license         = "BSD"
    generators      = "cmake"
    settings        = "arch", "build_type", "compiler", "os"
    options         = {"shared": [True, False]}
    default_options = "shared=True"
    short_paths     = True

    def source(self):
        zip_name = "v%s.zip" % self.version
        download ("%s/archive/%s" % (self.url, zip_name), zip_name, verify=False)
        unzip    (zip_name)
        os.unlink(zip_name)

    def build(self):
        cmake            = CMake(self)
        root_folder_name = "%s-%s" % (self.name, self.version)
        shared_options   = "-DBUILD_SHARED=ON" if self.options.shared else "-DBUILD_SHARED=OFF"
        self.run("cmake %s %s %s" % (root_folder_name, cmake.command_line, shared_options))
        self.run("cmake --build . %s" % cmake.build_config)

    def package(self):
        root_folder_name    = "%s-%s"       % (self.name, self.version)
        include_folder_name = "%s/headers/" % root_folder_name
        lib_folder_name     = "%s/lib/"     % root_folder_name
        bin_folder_name     = "%s/bin/"     % root_folder_name

        # Locate where OpenVR places the libraries and binaries.
        postfix = ""
        if self.settings.os == "Windows":
            postfix += "win"
        elif self.settings.os == "Macos":
            postfix += "osx"
        else:
            postfix += "linux"

        if self.settings.arch == "x86":
            postfix += "32"
        else:
            postfix += "64"
        if self.settings.os == "Windows":
            if self.settings.build_type == "Debug":
                postfix += "/Debug"
            else:
                postfix += "/Release"
        lib_folder_name += postfix
        bin_folder_name += postfix

        self.copy("*.h"    , dst="include", src=include_folder_name)
        self.copy("*.so"   , dst="lib"    , src=lib_folder_name, keep_path=False)
        self.copy("*.lib"  , dst="lib"    , src=lib_folder_name, keep_path=False)
        self.copy("*.dylib", dst="lib"    , src=lib_folder_name, keep_path=False)
        self.copy("*.lib"  , dst="lib"    , src=bin_folder_name, keep_path=False)
        self.copy("*.dll"  , dst="bin"    , src=bin_folder_name, keep_path=False)

    def package_info(self):
        self.cpp_info.libs = ["openvr_api"]
        if self.settings.os == "Windows" and self.settings.arch != "x86":
            self.cpp_info.libs = ["openvr_api64"]
