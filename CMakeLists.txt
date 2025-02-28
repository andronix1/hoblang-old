cmake_minimum_required(VERSION 3.19)
project(hoblang)

set(CMAKE_FIND_PACKAGE_SORT_ORDER NATURAL)
set(CMAKE_FIND_PACKAGE_SORT_DIRECTION DEC)
find_package(LLVM REQUIRED)

message(STATUS "LLVM version: ${LLVM_PACKAGE_VERSION}")

set(SOURCE_FILES
    src/main.c
    src/print.c

    src/cmd/api.c
    src/cmd/shared/arch.c
    src/cmd/build_exe.c
    src/cmd/build_obj.c
    src/cmd/emit_llvm.c
    src/cmd/help.c

    src/print/base.c
    src/print/lexer.c
	src/print/ast/expr.c
	src/print/ast/type.c
	src/print/sema/type.c

	src/core/slice/api.c
	src/core/slice/io.c
	src/core/path.c
	src/core/process.c
	src/core/vec.c
	src/core/print.c
	src/core/log.c

	src/lexer/api.c
	src/lexer/private.c
	src/lexer/parts/char.c
	src/lexer/parts/ident.c
	src/lexer/parts/integer.c
	src/lexer/parts/keyword.c
	src/lexer/parts/symbol.c
	src/lexer/parts/str.c
	
    src/ast/expr.c

	src/parser/api.c
	src/parser/private.c
	src/parser/parts/types/func.c
	src/parser/parts/stmts/var.c
	src/parser/parts/stmts/return.c
	src/parser/parts/stmts/while.c
	src/parser/parts/stmts/defer.c
	src/parser/parts/stmts/if_else.c
	src/parser/parts/stmts/asm.c
	src/parser/parts/stmts/loop_control.c
	src/parser/parts/decls/struct.c
	src/parser/parts/decls/ext_func.c
	src/parser/parts/decls/func_info.c
	src/parser/parts/decls/func.c
	src/parser/parts/module.c
	src/parser/parts/module_node.c
	src/parser/parts/type.c
	src/parser/parts/expr.c
	src/parser/parts/path.c
	src/parser/parts/body.c

    src/sema/arch/bits.c
    src/sema/arch/private.c
	src/sema/project/api.c
    src/sema/type/api.c
    src/sema/type/private.c
	src/sema/module/api.c
	src/sema/module/private.c
	src/sema/module/loop/api.c
	src/sema/module/decls/api.c
	src/sema/module/value/api.c
	src/sema/module/parts/decls/struct.c
	src/sema/module/parts/exprs/array.c
	src/sema/module/parts/exprs/as.c
	src/sema/module/parts/exprs/idx.c
	src/sema/module/parts/exprs/binop.c
	src/sema/module/parts/exprs/call.c
	src/sema/module/parts/exprs/int.c
	src/sema/module/parts/exprs/float.c
	src/sema/module/parts/exprs/not.c
	src/sema/module/parts/exprs/ref.c
	src/sema/module/parts/exprs/unary.c
	src/sema/module/parts/exprs/null.c
	src/sema/module/parts/exprs/ret_on_null.c
	src/sema/module/parts/exprs/get_local.c
	src/sema/module/parts/exprs/get_inner.c
	src/sema/module/parts/exprs/unwrap.c
	src/sema/module/parts/stmts/assign.c
	src/sema/module/parts/stmts/loop_control.c
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

	src/llvm/private.c
	src/llvm/api.c
	src/llvm/utils/member.c
	src/llvm/parts/types/slice.c
	src/llvm/parts/types/optional.c
	src/llvm/parts/path.c
	src/llvm/parts/type.c
	src/llvm/parts/module.c
	src/llvm/parts/body.c
	src/llvm/parts/stmts/asm/x86.c
	src/llvm/parts/stmts/assign.c
	src/llvm/parts/stmts/asm/x86.c
	src/llvm/parts/stmts/if.c
	src/llvm/parts/stmts/return.c
	src/llvm/parts/stmts/var.c
	src/llvm/parts/stmts/while.c
	src/llvm/parts/stmts/break.c
	src/llvm/parts/stmts/continue.c
	src/llvm/parts/expr.c
)

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
