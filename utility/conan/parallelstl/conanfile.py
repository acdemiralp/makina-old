from conans import ConanFile
from conans.tools import download, unzip
import os

class Project(ConanFile):
    name        = "parallelstl"
    description = "Parallel implementation of stl on top of tbb."
    version     = "20180619"
    url         = "https://github.com/intel/parallelstl"
    license     = "Apache 2.0"

    def source(self):
        zip_name = "%s.zip" % self.version
        download ("%s/archive/%s" % (self.url, zip_name), zip_name, verify=False)
        unzip    (zip_name)
        os.unlink(zip_name)

    def package(self):
        self.copy("*", dst="include", src=("%s-%s/include" % (self.name, self.version)))