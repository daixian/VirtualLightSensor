#!/usr/bin/env python3
# coding=utf-8
from conans import ConanFile, CMake, tools

import os
import sys
import io
# sys.stdout = io.TextIOWrapper(sys.stdout.buffer, encoding='gbk')

os.system("chcp 65001")


class VirtualLightSensorWinSerConan(ConanFile):
    name = "VirtualLightSensorWinSer"
    version = "1.0.0"
    license = "GPL"
    author = "daixian<amano_tooko@qq.com>"
    url = "https://github.com/daixian/VirtualLightSensor"
    description = "虚拟光线传感器windows服务"
    topics = ("opencv", "daixian")
    settings = "os", "compiler", "build_type", "arch"
    options = {"build_test": [True, False]}
    default_options = {"build_test": True,
                       "dlog:shared": True,
                       "opencv:shared": True,
                       "boost:without_test": True}
    generators = "cmake"
    exports_sources = "src/*"

    def requirements(self):
        pass

    def build_requirements(self):
        self.build_requires("cvsystem/[>=3.3.11]@daixian/stable")
        self.build_requires("gtest/1.8.1@bincrafters/stable")

    def _configure_cmake(self):
        '''
        转换python的设置到CMake
        '''
        cmake = CMake(self)
        cmake.definitions["VLS_WINSER_BUILD_TESTS"] = self.options.build_test
        return cmake

    def build(self):
        print("进入了build...")
        cmake = self._configure_cmake()
        cmake.configure(source_folder="src")
        cmake.build()

        # Explicit way:
        # self.run('cmake %s/hello %s'
        #          % (self.source_folder, cmake.command_line))
        # self.run("cmake --build . %s" % cmake.build_config)

    def package(self):
        self.copy("*.h", dst="include", src="src")
        self.copy("*.lib", dst="lib", keep_path=False)
        self.copy("*.dll", dst="bin", keep_path=False)
        self.copy("*.exe", dst="bin", keep_path=False)

        self.copy("*.dylib*", dst="lib", keep_path=False)
        self.copy("*.so", dst="lib", keep_path=False)
        self.copy("*.a", dst="lib", keep_path=False)
        self.copy("*.out", dst="lib", keep_path=False)

    def package_info(self):
        self.cpp_info.libs = ["VirtualLightSensorWinSer"]
