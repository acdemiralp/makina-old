from   conans             import ConanFile, CMake, tools
from   conans.tools       import download, unzip
from   distutils.dir_util import copy_tree
import os

class Project(ConanFile):
    name            = "uWebSockets"
    description     = "Conan package for uWebSockets."
    version         = "0.14.8"                
    url             = "https://github.com/uNetworking/uWebSockets"
    cmake_name      = "uWS_cmake"
    cmake_version   = "1.2.0"
    cmake_url       = "https://github.com/acdemiralp/uWS_cmake"
    settings        = "arch", "build_type", "compiler", "os"
    generators      = "cmake"
    requires        = (("OpenSSL/1.1.0g@conan/stable"),
                       ("zlib/1.2.8@conan/stable"    ))
    options         = {"shared": [True, False], "use_asio": [True, False], "use_libuv": [True, False]} 
    default_options = "shared=False", "use_asio=False", "use_libuv=True"

    def imports(self):
        self.copy("*.dylib*", dst="", src="lib")
        self.copy("*.dll"   , dst="", src="bin")

    def configure(self):
        self.options["OpenSSL"].shared = self.options.shared
        self.options["zlib"   ].shared = self.options.shared
    
    def requirements(self):
        if self.options.use_asio:
            self.requires("boost_asio/1.67.0@bincrafters/testing")
        if self.options.use_libuv:
            self.requires("libuv/1.15.0@bincrafters/stable")
            if self.options.shared:
                self.options["libuv"].shared = self.options.shared

    def source(self):
        extension  = "zip" if self.settings.os == "Windows" else "tar.gz" 
        zip_name   = "v%s.%s" % (self.version, extension)
        download   ("%s/archive/%s" % (self.url, zip_name), zip_name, verify=False)
        unzip      (zip_name)
        os.unlink  (zip_name)

        zip_name   = "%s.zip" % self.cmake_version
        download   ("%s/archive/%s" % (self.cmake_url, zip_name), zip_name, verify=False)
        unzip      (zip_name)
        os.unlink  (zip_name)
        copy_tree  (("%s-%s" % (self.cmake_name, self.cmake_version)), ("%s-%s" % (self.name, self.version)))

    def build(self):
        cmake          = CMake(self)
        shared_options = "-DBUILD_SHARED_LIBS=ON" if self.options.shared else "-DBUILD_SHARED_LIBS=OFF"
        asio_options   = "-DUSE_ASIO=ON" if self.options.use_asio else "-DUSE_ASIO=OFF"
        libuv_options  = "-DUSE_LIBUV=ON" if self.options.use_libuv else "-DUSE_LIBUV=OFF"
        self.run("cmake %s-%s %s %s %s %s" % (self.name, self.version, cmake.command_line, shared_options, asio_options, libuv_options))
        self.run("cmake --build . %s" % cmake.build_config)

    def package(self):
        include_folder = "%s-%s/src" % (self.name, self.version)
        self.copy("*.h"     , dst="include", src=include_folder)
        self.copy("*.hpp"   , dst="include", src=include_folder)
        self.copy("*.inl"   , dst="include", src=include_folder)
        self.copy("*.dylib*", dst="lib"    , keep_path=False   )
        self.copy("*.lib"   , dst="lib"    , keep_path=False   )
        self.copy("*.so*"   , dst="lib"    , keep_path=False   )
        self.copy("*.dll"   , dst="bin"    , keep_path=False   )

    def package_info(self):
        self.cpp_info.libs = [self.name]