cmake_minimum_required(VERSION 3.7)
project(hoblang)

set(SOURCE_FILES
    src/main.c
	src/tokenizer/tokenizer.c
	src/tokenizer/print.c
	src/tokenizer/base.c
	src/lexer/base.c
    src/core/fatptr.c
    src/core/vec.c
)

add_executable(hoblang ${SOURCE_FILES})
target_compile_options(hoblang PRIVATE -Wall -Wextra -Wpedantic -Werror)
target_include_directories(hoblang PRIVATE src)
