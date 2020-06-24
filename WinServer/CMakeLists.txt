cmake_minimum_required(VERSION 3.1.0)

#增加这个wrapper工程让vscode的cmake插件可以工作
project(VLSWinSer_wrapper VERSION 1.0.0 LANGUAGES CXX)

message(STATUS "CMAKE_BINARY_DIR = ${CMAKE_BINARY_DIR}")

#从build目录里去得到conanbuildinfo.cmake
set(CONAN_BUILD_INFO_PATH ${CMAKE_CURRENT_SOURCE_DIR}/build/conanbuildinfo.cmake)
# include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
include(${CONAN_BUILD_INFO_PATH})
conan_basic_setup()

include(CheckIncludeFile)

# 防止vs工程install报错
# 修改一些默认值 Use this snippet *after* PROJECT(xxx):
if(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
    set(CMAKE_INSTALL_PREFIX ${CMAKE_SOURCE_DIR}/install CACHE PATH "Install directory" FORCE)
endif(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)

# 现在使用了conan库,这里并不需要了
if(WIN32)
# # 设置编译选项为MT https://stackoverflow.com/questions/14172856/compile-with-mt-instead-of-md-using-cmake
    # set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MT")
    # 控制一下调试时候Debug的VC编译方式
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MD")
endif()

add_subdirectory("src")
