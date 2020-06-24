cmake_minimum_required(VERSION 3.8 FATAL_ERROR)

set(CMAKE_CXX_STANDARD 14)
set(SUB_NAME "test.out")

add_definitions(-D UNICODE)
if(WIN32)
    add_definitions(-D _WINDOWS)
endif()

FILE(GLOB_RECURSE SOURCE_LIST "*.cpp" "*.h" "*.hpp" "*.c")
message(STATUS "SOURCE_LIST = ${SOURCE_LIST}")
add_executable(${SUB_NAME}
                ${SOURCE_LIST})
 
find_package (Threads)

# Prevent GoogleTest from overriding our compiler/linker options
# when building with Visual Studio
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

# CMake 2.8.11 之后gtest是自动添加的，所以下面的没有执行
if (CMAKE_VERSION  VERSION_LESS 2.8.11)
    include_directories("${gtest_SOURCE_DIR}/include"
                        "${gmock_SOURCE_DIR}/include")
endif()

if(WIN32)
    if(DEFINED CONAN_BIN_DIRS_OPENCV)
        add_custom_command(TARGET ${SUB_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory "${CONAN_BIN_DIRS_OPENCV}" ${EXECUTABLE_OUTPUT_PATH}) #拷贝整个文件夹算了
    endif()
    if(DEFINED CONAN_BIN_DIRS_DLOG)
        add_custom_command(TARGET ${SUB_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory "${CONAN_BIN_DIRS_DLOG}" ${EXECUTABLE_OUTPUT_PATH}) #拷贝整个文件夹算了
    endif()
endif()

find_package (Threads)

# Dependencies
target_include_directories(${SUB_NAME} PRIVATE  ${CMAKE_SOURCE_DIR} "${CMAKE_SOURCE_DIR}/..")

# 这里exe链接不进来,One may link only to INTERFACE, OBJECT, STATIC or SHARED libraries                                               
target_link_libraries(${SUB_NAME} PRIVATE ${CONAN_LIBS} ${CMAKE_THREAD_LIBS_INIT})

add_test(NAME AllCppTest COMMAND ${SUB_NAME})