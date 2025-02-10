cmake_minimum_required(VERSION 3.19)
project(hoblang)

set(CMAKE_FIND_PACKAGE_SORT_ORDER NATURAL)
set(CMAKE_FIND_PACKAGE_SORT_DIRECTION DEC)
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
	src/lexer/impl/api.c
	src/lexer/impl/next.c
	src/lexer/impl/private.c
	src/lexer/parts/char.c
	src/lexer/parts/ident.c
	src/lexer/parts/integer.c
	src/lexer/parts/keyword.c
	src/lexer/parts/symbol.c
	src/lexer/parts/str.c
	
    src/ast/expr.c
	src/ast/print/expr.c
	src/ast/print/type.c

	src/parser/impl/api.c
	src/parser/impl/private.c
	src/parser/parts/stmts/var.c
	src/parser/parts/stmts/return.c
	src/parser/parts/stmts/while.c
	src/parser/parts/stmts/defer.c
	src/parser/parts/stmts/if_else.c
	src/parser/parts/stmts/asm.c
	src/parser/parts/decls/ext_func.c
	src/parser/parts/decls/func.c
	src/parser/parts/module.c
	src/parser/parts/module_node.c
	src/parser/parts/type.c
	src/parser/parts/expr.c
	src/parser/parts/path.c
	src/parser/parts/body.c

	src/sema/module.c
	src/sema/project.c
	src/sema/type.c
	src/sema/parts/expr/eval.c
	src/sema/parts/exprs/array.c
	src/sema/parts/exprs/as.c
	src/sema/parts/exprs/binop.c
	src/sema/parts/exprs/call.c
	src/sema/parts/exprs/int.c
	src/sema/parts/exprs/not.c
	src/sema/parts/exprs/ref.c
	src/sema/parts/exprs/unary.c
	src/sema/parts/exprs/get_local.c
	src/sema/parts/exprs/get_inner.c
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
    -O3 # -g -ggdb
	-Wall -Wextra -Werror
	-Wno-format
	-Wno-error=unused-parameter -Wno-error=unused-variable
)
target_link_libraries(hoblang PRIVATE LLVM)
target_include_directories(hoblang PRIVATE src)
