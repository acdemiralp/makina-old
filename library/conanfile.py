from   conans       import ConanFile, CMake, tools
from   conans.tools import download, unzip
import os

class MakinaConan(ConanFile):
    name            = "makina"
    description     = "Conan package for makina."    
    version         = "1.0.0"            
    url             = "https://github.com/acdemiralp/makina"
    license         = "MIT"											
    settings        = "arch", "build_type", "compiler", "os"
    generators      = "cmake"
    requires        = (("assimp/4.1.0@RWTH-VR/thirdparty"      ),
                       ("bgfx/master@acdemiralp/makina"        ),
                       ("boost/1.66.0@conan/stable"            ),
                       ("bm/1.0.1@acdemiralp/makina"           ),
                       ("bullet/2.87@bincrafters/stable"       ),
                       ("catch2/2.2.0@bincrafters/stable"      ),
                       ("cppzmq/4.2.2@bincrafters/stable"      ),
                       ("di/1.2.0@acdemiralp/makina"           ),
                       ("ec/1.3.0@acdemiralp/makina"           ),
                       ("eigen/3.3.4@conan/stable"             ),
                       ("fg/1.1.0@acdemiralp/makina"           ),
                       ("fi/1.0.2@acdemiralp/makina"           ),
                       ("freetype/2.8.1@bincrafters/stable"    ),
                       ("gl/1.1.2@acdemiralp/makina"           ),
                       ("glm/0.9.8.5@g-truc/stable"            ),
                       ("hdf5/1.10.2@acdemiralp/makina"        ),
                       ("HighFive/1.5@acdemiralp/makina"       ),
                       ("im3d/master@acdemiralp/makina"        ),
                       ("imgui/1.53@acdemiralp/makina"         ),
                       ("jsonformoderncpp/3.1.1@vthiery/stable"),
                       ("memory/0.6-1@acdemiralp/makina"       ),
                       ("ospray/1.6.0@acdemiralp/makina"       ),
                       ("ra/1.0.0@acdemiralp/makina"           ),
                       ("rttr/0.9.5@acdemiralp/makina"         ),
                       ("spdlog/0.16.3@bincrafters/stable"     ),
                       ("splinter/3.0@acdemiralp/makina"       ),
                       ("stb/20180214@conan/stable"            ),
                       ("TBB/2018_U3@RWTH-VR/thirdparty"       ),
                       ("uWebSockets/0.14.8@acdemiralp/makina" ),
                       ("vkhlf/master@acdemiralp/makina"       ),
                       ("zlib/1.2.8@conan/stable"              )) 
    options         = {"shared": [True, False]} 
    default_options = "shared=True"
    
    def configure(self):
       self.options["gl"].shared = False

    def imports(self):
       self.copy("*.dylib*", dst="", src="lib")
       self.copy("*.dll"   , dst="", src="bin")
       
    def source(self):
        zip_name = "v%s.zip" % self.version
        download ("%s/archive/%s" % (self.url, zip_name), zip_name, verify=False)
        unzip    (zip_name)
        os.unlink(zip_name)

    def build(self):
        cmake          = CMake(self)
        shared_options = "-DBUILD_SHARED_LIBS=ON" if self.options.shared else "-DBUILD_SHARED_LIBS=OFF"
        self.run("cmake %s-%s %s %s" % (self.name, self.version, cmake.command_line, shared_options))
        self.run("cmake --build . %s" % cmake.build_config)

    def package(self):
        include_folder = "%s-%s/include" % (self.name, self.version)
        self.copy("*.h"     , dst="include", src=include_folder)
        self.copy("*.hpp"   , dst="include", src=include_folder)
        self.copy("*.inl"   , dst="include", src=include_folder)
        self.copy("*.dylib*", dst="lib"    , keep_path=False   )
        self.copy("*.lib"   , dst="lib"    , keep_path=False   )
        self.copy("*.so*"   , dst="lib"    , keep_path=False   )
        self.copy("*.dll"   , dst="bin"    , keep_path=False   )

    def package_info(self):
        self.cpp_info.libs = [self.name]
        if self.settings.os == "Windows":
            if not self.options.shared:
                self.cpp_info.defines.append("%s_STATIC" % self.name.upper())