cmake_minimum_required (VERSION 3.20)
project(curl-example VERSION 1.0.0 DESCRIPTION "Example of using cURL")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(PkgConfig REQUIRED)
pkg_check_modules(LIBCURL REQUIRED libcurl)

add_executable(download
    src/curl_wrapper.cpp
    src/download.cpp)

target_include_directories(download PRIVATE ${LIBCURL_INCLUDE_DIRS})
target_compile_options(download PRIVATE ${LIBCURL_CFLAGS}
    -Werror -Wall -Wextra -Wpedantic)
target_link_libraries(download PRIVATE ${LIBCURL_LIBRARIES})
