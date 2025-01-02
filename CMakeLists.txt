cmake_minimum_required(VERSION 3.7)
project(hoblang)

set(SOURCE_FILES
    src/main.c
	src/core/fatptr.c
	src/core/vec.c
	src/core/log.c
	src/lexer/tokens.c
	src/lexer/lexer.c
	src/lexer/base.c
	src/parser/ast.c
	src/parser/base.c
	src/parser/parser.c
	src/parser/expr.c
)

add_executable(hoblang ${SOURCE_FILES})
target_compile_options(hoblang PRIVATE 
	-Wall -Wextra -Wpedantic -Werror
	-Wno-format
	-Wno-error=unused-parameter -Wno-error=unused-variable
)
target_include_directories(hoblang PRIVATE src)
