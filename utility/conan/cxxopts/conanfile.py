from conans import ConanFile
from conans.tools import download, unzip
import os

class Project(ConanFile):
    name        = "cxxopts"
    description = "Lightweight C++ command line option parser."
    version     = "2.1.0"
    url         = "https://github.com/jarro2783/cxxopts"
    license     = "MIT"

    def source(self):
        zip_name = "v%s.zip" % self.version
        download ("%s/archive/%s" % (self.url, zip_name), zip_name, verify=False)
        unzip    (zip_name)
        os.unlink(zip_name)

    def package(self):
        self.copy("*.hpp", dst="include", src=("%s-%s/include" % (self.name, self.version)))