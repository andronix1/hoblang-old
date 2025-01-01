cmake_minimum_required(VERSION 3.7)
project(hoblang)

set(SOURCE_FILES
    src/main.c
	src/tokenizer/tokens.c
	src/tokenizer/tokenizer.c
	src/tokenizer/print.c
	src/tokenizer/base.c
	src/lexer/base.c
	src/lexer/lexer.c
    src/lexer/expr.c
    src/core/fatptr.c
    src/core/vec.c
)

add_executable(hoblang ${SOURCE_FILES})
target_compile_options(hoblang PRIVATE 
	-Wall -Wextra -Wpedantic -Werror
	-Wno-format
	-Wno-error=unused-parameter -Wno-error=unused-variable
)
target_include_directories(hoblang PRIVATE src)
