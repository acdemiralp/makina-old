from   conans       import ConanFile, CMake, tools
from   conans.tools import download, unzip
import os

class Project(ConanFile):
    name        = "ospray"
    version     = "1.6.0"     
    description = "Conan package for ospray raytracing framework."           
    url         = "https://github.com/ospray/ospray"
    license     = "Apache 2.0"                                         
    settings    = "arch", "build_type", "compiler", "os"
    root_folder = ""

    def source(self):
        if self.settings.os == "Windows":
            suffix    = "windows"
            extension = "zip"
        elif self.settings.os == "Macos":
            suffix    = "x86_64.macosx"
            extension = "tar.gz"
        else:
            suffix    = "x86_64.linux"
            extension = "tar.gz"

        self.root_folder = "%s-%s.%s" % (self.name, self.version, suffix)
        download_url     = "%s/releases/download/v%s/%s.%s" % (self.url, self.version, self.root_folder, extension)
        zip_name         = "%s.zip" % self.version
        download (download_url, zip_name, verify=False)
        unzip    (zip_name)
        os.unlink(zip_name)

    def package(self):
        include_folder = "%s/include" % self.root_folder
        self.copy("*.h"     , dst="include", src=include_folder)
        self.copy("*.hpp"   , dst="include", src=include_folder)
        self.copy("*.inl"   , dst="include", src=include_folder)
        self.copy("*.dylib*", dst="lib"    , keep_path=False   )
        self.copy("*.lib"   , dst="lib"    , keep_path=False   )
        self.copy("*.so*"   , dst="lib"    , keep_path=False   )
        self.copy("*.dll"   , dst="bin"    , keep_path=False   )

    def package_info(self):
        if self.settings.os == "Windows":
            self.cpp_info.libs = [
                "ospray"                , 
                "ospray_common"         , 
                "ospray_module_ispc"    , 
                "ospray_sg"             , 
                "ospray_tfn"            ]
        else:
            self.cpp_info.libs = [
                "libtbb"                , 
                "libtbbmalloc"          ,
                "libembree3"            , 
                "libospray"             , 
                "libospray_common"      , 
                "libospray_module_ispc" , 
                "libospray_sg"          , 
                "libospray_tfn"         ]