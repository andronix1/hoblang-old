cmake_minimum_required(VERSION 3.7)
project(hoblang)

find_package(LLVM REQUIRED)

message(STATUS "LLVM version: ${LLVM_PACKAGE_VERSION}")
message(STATUS "LLVM id: ${LLVM_LIBRARIES}")

set(SOURCE_FILES
    src/main.c
	src/core/fatptr.c
	src/core/vec.c
	src/core/log.c

	src/lexer/tokens.c
	src/lexer/lexer.c
	src/lexer/lex.c
	src/lexer/parsers/char.c
	src/lexer/parsers/ident.c
	src/lexer/parsers/integer.c
	src/lexer/parsers/keyword.c
	src/lexer/parsers/symbol.c

	src/parser/ast.c
	src/parser/base.c
	src/parser/parser.c
	src/parser/expr.c
	src/sema/base.c
	src/sema/sema.c
	src/sema/type.c	
	src/llvm/llvm.c
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
