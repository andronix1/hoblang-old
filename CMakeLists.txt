cmake_minimum_required(VERSION 3.7)
project(hoblang)

set(SOURCE_FILES
    src/main.c
	src/tokenizer/tokenizer.c
    src/core/file.c
    src/core/tx_buffer.c
    src/core/ftx.c
    src/core/fatptr.c
)

add_executable(hoblang ${SOURCE_FILES})
target_compile_options(hoblang PRIVATE -Wall -Wextra -Wpedantic -Werror)
target_include_directories(hoblang PRIVATE src)
