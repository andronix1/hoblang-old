cmake_minimum_required(VERSION 3.7)
project(hoblang)

find_package(LLVM REQUIRED)

message(STATUS "LLVM version: ${LLVM_PACKAGE_VERSION}")

set(SOURCE_FILES
    src/main.c

	src/core/vec.c
	src/core/slice.c
	src/core/print.c
	src/core/log.c

	src/lexer/tokens.c
	src/lexer/lexer.c
	src/lexer/lex.c
	src/lexer/parsers/char.c
	src/lexer/parsers/ident.c
	src/lexer/parsers/integer.c
	src/lexer/parsers/keyword.c
	src/lexer/parsers/symbol.c

	src/parser/parser.c
	src/parser/parsers/module.c
	src/parser/parsers/module_node.c
	src/parser/parsers/type.c
	src/parser/parsers/func_call.c
	src/parser/parsers/expr.c
	src/parser/parsers/body.c

	src/sema/sema.c
	src/sema/type.c
	src/sema/parts/module.c
	src/sema/parts/type.c
	src/sema/parts/body.c
	src/sema/parts/expr.c

	src/llvm/llvm.c
	src/llvm/parts/module.c
	src/llvm/parts/body.c
	src/llvm/parts/expr.c
)

include_directories(${LLVM_INCLUDE_DIRS})
link_directories(${LLVM_LIBRARY_DIRS})
add_executable(hoblang ${SOURCE_FILES})
target_compile_options(hoblang PRIVATE 
	-Wall -Wextra -Wpedantic -Werror
	-Wno-format
	-Wno-error=unused-parameter -Wno-error=unused-variable
)
target_link_libraries(hoblang PRIVATE LLVM)
target_include_directories(hoblang PRIVATE src)
