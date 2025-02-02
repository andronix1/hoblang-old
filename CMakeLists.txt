cmake_minimum_required(VERSION 3.7)
project(hoblang)

find_package(LLVM REQUIRED)

message(STATUS "LLVM version: ${LLVM_PACKAGE_VERSION}")

set(SOURCE_FILES
    src/main.c

	src/core/path.c
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
	src/lexer/parsers/str.c

	src/parser/parser.c
	src/parser/print/expr.c
	src/parser/print/type.c
	src/parser/ast/expr.c
	src/parser/parsers/stmts/var.c
	src/parser/parsers/stmts/return.c
	src/parser/parsers/stmts/while.c
	src/parser/parsers/stmts/defer.c
	src/parser/parsers/stmts/if_else.c
	src/parser/parsers/stmts/asm.c
	src/parser/parsers/decls/ext_func.c
	src/parser/parsers/decls/func.c
	src/parser/parsers/module.c
	src/parser/parsers/module_node.c
	src/parser/parsers/type.c
	src/parser/parsers/func_call.c
	src/parser/parsers/expr.c
	src/parser/parsers/path.c
	src/parser/parsers/body.c

	src/sema/module.c
	src/sema/project.c
	src/sema/type.c
	src/sema/parts/exprs/array.c
	src/sema/parts/exprs/as.c
	src/sema/parts/exprs/binop.c
	src/sema/parts/exprs/call.c
	src/sema/parts/exprs/int.c
	src/sema/parts/stmts/assign.c
	src/sema/parts/stmts/asm.c
	src/sema/parts/stmts/if_else.c
	src/sema/parts/stmts/return.c
	src/sema/parts/stmts/defer.c
	src/sema/parts/stmts/var.c
	src/sema/parts/stmts/while.c
	src/sema/parts/module.c
	src/sema/parts/path.c
	src/sema/parts/type.c
	src/sema/parts/body.c
	src/sema/parts/expr.c

	src/llvm/llvm.c
	src/llvm/utils/slices.c
	src/llvm/parts/path.c
	src/llvm/parts/module.c
	src/llvm/parts/body.c
	src/llvm/parts/stmts/asm/x86.c
	src/llvm/parts/stmts/assign.c
	src/llvm/parts/stmts/asm/x86.c
	src/llvm/parts/stmts/if.c
	src/llvm/parts/stmts/return.c
	src/llvm/parts/stmts/var.c
	src/llvm/parts/stmts/while.c
	src/llvm/parts/expr.c
)

include_directories(${LLVM_INCLUDE_DIRS})
link_directories(${LLVM_LIBRARY_DIRS})
add_executable(hoblang ${SOURCE_FILES})
target_compile_options(hoblang PRIVATE 
	-Wall -Wextra -Werror
	-Wno-format
	-Wno-error=unused-parameter -Wno-error=unused-variable
)
target_link_libraries(hoblang PRIVATE LLVM)
target_include_directories(hoblang PRIVATE src)
