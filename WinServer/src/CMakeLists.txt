cmake_minimum_required(VERSION 3.1.0)

# 在上面设置工具链之后下面要立马project命令
project(VLSWinSerApp VERSION 2.0.0 LANGUAGES CXX)

set(SUB_DIR1 "VLSWinSer")
# set(CMAKE_CXX_STANDARD 14)

if (NOT DEFINED CONAN_BUILD_INFO_PATH)
    # 在有了project之后需要第一时间搞个conan设置吧
    include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
    conan_basic_setup()
    # conan_set_vs_runtime()
endif()

# 设置是否编译Test
option(VLS_WINSER_BUILD_TESTS "VLS_WINSER_BUILD_TESTS" ON)
message(STATUS "VLS_WINSER_BUILD_TESTS = ${VLS_WINSER_BUILD_TESTS}" )
# 设置是否编译成共享库
option(VLS_WINSER_BUILD_SHARED "VLS_WINSER_BUILD_SHARED" OFF)
message(STATUS "VLS_WINSER_BUILD_SHARED = ${VLS_WINSER_BUILD_SHARED}" )

add_definitions(-D DLOG_UTF8)
add_definitions(-D _NO_ASYNCRTIMP)

# 设置生成一个VS工程里的文件夹
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

# 修改一些默认值 Use this snippet *after* PROJECT(xxx):
if(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
    set(CMAKE_INSTALL_PREFIX ${CMAKE_SOURCE_DIR}/install CACHE PATH "Install directory" FORCE)
endif(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)


#设置文件输出的目录
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}/lib)
set(RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

add_subdirectory(${SUB_DIR1})

if(VLS_WINSER_BUILD_TESTS)
enable_testing()
include(CTest)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/test)
endif()