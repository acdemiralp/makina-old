from   conans        import AutoToolsBuildEnvironment, ConanFile, tools
from   conans.errors import ConanException
from   conans.tools  import download, os_info, unzip
import os
import shutil

class Project(ConanFile):
    name            = "hdf5"
    version         = "1.10.2"     
    description     = "Conan package for HDF5."           
    url             = "https://portal.hdfgroup.org/display/support"
    license         = "BSD"                                         
    settings        = "arch", "build_type", "compiler", "os"
    generators      = "cmake"
    options         = {"shared": [True, False]}
    default_options = "shared=True"
    requires        = "zlib/1.2.11@conan/stable"
    exports         = ["HDF5options.cmake"]

    def source(self):
        download_url    = "https://support.hdfgroup.org/ftp/HDF5/releases/hdf5-1.10/hdf5-%s/src/CMake-hdf5-%s%s" % (self.version, self.version, ".zip" if os_info.is_windows else "tar.gz")
        source_zip_name = "%s.zip" % self.version
        download (download_url, source_zip_name, verify=False)
        unzip    (source_zip_name)
        os.unlink(source_zip_name)

    def build(self):
        os    .chdir   (os.path.join(self.source_folder, ("CMake-hdf5-%s" % self.version)))
        shutil.copyfile(os.path.join(self.source_folder, "HDF5options.cmake"), "HDF5options.cmake")

        build_generator = "Unix"
        if self.settings.compiler == "Visual Studio":
            if self.settings.compiler.version == "12":
                build_generator = "VS201364"
            if self.settings.compiler.version == "14":
                build_generator = "VS201564"
            if self.settings.compiler.version == "15":
                build_generator = "VS201764"

        try:
            self.run("ctest -S HDF5config.cmake,BUILD_GENERATOR=%s -C %s -V -O hdf5.log" % (build_generator, self.settings.build_type))
        except ConanException:
            pass # Allowed to fail: The intent is to build the library, not to run the tests.

        install_folder = "HDF5-%s-%s" % (self.version, "win64" if os_info.is_windows else "Linux")
        install_zip    = "%s.zip" % install_folder
        os.chdir       ("build")
        unzip          (install_zip)
        os.unlink      (install_zip)
        shutil.copytree(install_folder, os.path.join("..", "..", "install"))

    def package(self):
        self.copy("*", dst="include", src="install/include")
        self.copy("*", dst="lib"    , src="install/lib"    )
        self.copy("*", dst="bin"    , src="install/bin"    )

    def package_info(self):
        if self.options.shared:
            self.cpp_info.libs = ["hdf5", "hdf5_hl"]
            self.cpp_info.defines.append("H5_BUILT_AS_DYNAMIC_LIB")
        else:
            self.cpp_info.libs = ["libhdf5", "libhdf5_hl"]
