cmake_minimum_required(VERSION 3.16)

set(IDF_TOOLCHAIN "clang")
set(EXTRA_COMPONENT_DIRS "./src")

include($ENV{IDF_PATH}/tools/cmake/project.cmake)
project(can-esp32-s3)
