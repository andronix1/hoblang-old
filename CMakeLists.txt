cmake_minimum_required(VERSION 3.19)
project(hoblang)

set(CMAKE_FIND_PACKAGE_SORT_ORDER NATURAL)
set(CMAKE_FIND_PACKAGE_SORT_DIRECTION DEC)
find_package(LLVM REQUIRED)

message(STATUS "LLVM version: ${LLVM_PACKAGE_VERSION}")

set(SOURCE_FILES
    src/main.c
    src/print.c

    src/print/base.c

	src/core/slice/api.c
	src/core/slice/io.c

	src/core/path.c
	src/core/vec.c
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

	src/sema/project/impl/api.c
    src/sema/type/api.c
    src/sema/type/print.c
    src/sema/type/private.c
	src/sema/module/impl/api.c
	src/sema/module/impl/private.c
	src/sema/module/parts/expr/eval.c
	src/sema/module/parts/exprs/array.c
	src/sema/module/parts/exprs/as.c
	src/sema/module/parts/exprs/binop.c
	src/sema/module/parts/exprs/call.c
	src/sema/module/parts/exprs/int.c
	src/sema/module/parts/exprs/not.c
	src/sema/module/parts/exprs/ref.c
	src/sema/module/parts/exprs/unary.c
	src/sema/module/parts/exprs/get_local.c
	src/sema/module/parts/exprs/get_inner.c
	src/sema/module/parts/stmts/assign.c
	src/sema/module/parts/stmts/asm.c
	src/sema/module/parts/stmts/if_else.c
	src/sema/module/parts/stmts/return.c
	src/sema/module/parts/stmts/defer.c
	src/sema/module/parts/stmts/var.c
	src/sema/module/parts/stmts/while.c
	src/sema/module/parts/module.c
	src/sema/module/parts/path.c
	src/sema/module/parts/type.c
	src/sema/module/parts/body.c
	src/sema/module/parts/expr.c

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
target_include_directories(hoblang PRIVATE include)
