cmake_minimum_required(VERSION 3.19)
project(hoblang)

set(CMAKE_FIND_PACKAGE_SORT_ORDER NATURAL)
set(CMAKE_FIND_PACKAGE_SORT_DIRECTION DEC)
find_package(LLVM REQUIRED)

message(STATUS "LLVM version: ${LLVM_PACKAGE_VERSION}")

file(GLOB_RECURSE SOURCE_FILES CONFIGURE_DEPENDS "src/*.c")

include_directories(${LLVM_INCLUDE_DIRS})
link_directories(${LLVM_LIBRARY_DIRS})
add_executable(hoblang ${SOURCE_FILES})
target_compile_options(hoblang PRIVATE 
    -O3 -g -ggdb
    -fsanitize=address
	-Wall -Wextra -Werror
	-Wno-format
	-Wno-error=unused-parameter -Wno-error=unused-variable
)
target_link_options(hoblang PRIVATE
    -fsanitize=address
)
target_link_libraries(hoblang PRIVATE LLVM)
target_include_directories(hoblang PRIVATE src)
target_include_directories(hoblang PRIVATE include)
