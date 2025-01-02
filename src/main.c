#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <errno.h>
#include "lexer/lexer.h"
#include "parser/parser.h"

const char *args_shift(int *argc, const char ***argv) {
    *argc -= 1;
    const char *cached = (*argv)[0];
    *argv = &(*argv)[1];
    return cached;
}

int main(int argc, const char **argv) {
	fatptr_register_printf();
	token_register_printf();
    args_shift(&argc, &argv);
    const char *command = args_shift(&argc, &argv);
    if (!strcmp(command, "lex")) {
		const char *file_path = "../examples/test.hob";
        Lexer lexer;
		if (!lexer_init(&lexer, file_path)) {
			return 1;
		}
        hob_log(LOGI, "lexed!");
		Parser parser;
		if (!parser_init(&parser, &lexer)) {
			return 1;
		}
		AstModule module;
		if (!parse_module(&parser, &module)) {
			return 1;
		}
        hob_log(LOGI, "parsed!");
		ast_print_module(&module);
		lexer_free(&lexer);
    } else {
        hob_log(LOGE, "unknown command!");
    }
    return 0;
}
