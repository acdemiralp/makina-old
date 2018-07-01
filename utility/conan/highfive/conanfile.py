from conans import ConanFile
from conans.tools import download, unzip
import os

class HighFiveConan(ConanFile):
    name        = "HighFive"
    description = "Header-only C++ HDF5 interface."
    version     = "1.5"
    url         = "https://github.com/BlueBrain/HighFive"
    license     = "MIT"

    def source(self):
        zip_name = "v%s.zip" % self.version
        download ("%s/archive/%s" % (self.url, zip_name), zip_name, verify=False)
        unzip    (zip_name)
        os.unlink(zip_name)

    def package(self):
        self.copy("*.hpp", dst="include", src=("%s-%s/include" % (self.name, self.version)))