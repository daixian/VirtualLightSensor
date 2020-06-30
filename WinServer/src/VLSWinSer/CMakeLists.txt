cmake_minimum_required(VERSION 3.8 FATAL_ERROR)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(SUB_NAME "VLSWinSer.out")

# add_definitions(-D UNICODE)
if(WIN32)
    add_definitions(-D _WINDOWS)
    add_definitions(-D _USERDLL)
    # 这个设置只支持windows，所以不要使用了
    # add_definitions(-D SPDLOG_WCHAR_TO_UTF8_SUPPORT)
endif()

# set(SOURCE_LIST
#     main.cpp
# )
# 先自动添加算了,这里要注意不知道为什么.c文件在add_executable()里没有添加.
FILE(GLOB_RECURSE SOURCE_LIST "*.cpp" "*.h" "*.hpp" "*.cc")

# 这个函数在VS工程里生成一个文件路径
function(assign_source_group)
    foreach(_source IN ITEMS ${ARGN})
        if (IS_ABSOLUTE "${_source}")
            file(RELATIVE_PATH _source_rel "${CMAKE_CURRENT_SOURCE_DIR}" "${_source}")
        else()
            set(_source_rel "${_source}")
        endif()
        get_filename_component(_source_path "${_source_rel}" PATH)
        string(REPLACE "/" "\\" _source_path_msvc "${_source_path}")
        #这个表示在VS里划分一个组
        source_group("${_source_path_msvc}" FILES "${_source}")
    endforeach()
endfunction(assign_source_group)

assign_source_group(${SOURCE_LIST})
message(STATUS "SOURCE_LIST = ${SOURCE_LIST}")

add_executable(${SUB_NAME}
               ${SOURCE_LIST})
 
find_package (Threads)

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

message(STATUS "CMAKE_SOURCE_DIR = ${CMAKE_SOURCE_DIR}")
message(STATUS "CONAN_LIBS = ${CONAN_LIBS}")

target_include_directories(${SUB_NAME} PRIVATE ${CMAKE_SOURCE_DIR})
target_link_libraries(${SUB_NAME} PRIVATE ${CONAN_LIBS} ${CMAKE_THREAD_LIBS_INIT})


# # 全都安装到根目录
# install(TARGETS ${SUB_NAME}
#        RUNTIME DESTINATION "./"
#        LIBRARY DESTINATION "./"
#        ARCHIVE DESTINATION "./"
# )
# 安装头文件
# install(FILES "dlog.h" DESTINATION "./")
# install(FILES "../UnitTest/DLog.cs" DESTINATION "./")

# Configuration
# set(generated_dir "${CMAKE_CURRENT_BINARY_DIR}/generated")
# set(version_config "${generated_dir}/${PROJECT_NAME}ConfigVersion.cmake")
# set(project_config "${generated_dir}/${PROJECT_NAME}Config.cmake")
# set(TARGETS_EXPORT_NAME "${PROJECT_NAME}Targets")
