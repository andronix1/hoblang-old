#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <errno.h>
#include "lexer/lexer.h"
#include "tokenizer/tokenizer.h"

const char *args_shift(int *argc, const char ***argv) {
    *argc -= 1;
    const char *cached = (*argv)[0];
    *argv = &(*argv)[1];
    return cached;
}

int main(int argc, const char **argv) {
	token_register_printf();
    args_shift(&argc, &argv);
    const char *command = args_shift(&argc, &argv);
    if (!strcmp(command, "tokens")) {
		const char *file_path = "../examples/test.hob";
        FILE *file = fopen(file_path, "r");
        if (!file) {
			printf("failed to open file `%s`: %s\n", file_path, strerror(errno));
            return 1;
        }
		FatPtr content;
		if (!fatptr_read_all(&content, file)) {
			fclose(file);
			return 1;
		}
        Tokenizer tokenizer = tokenizer_new(content);
		Tokens tokens = tokens_new();
		if (!tokenize_all(&tokenizer , &tokens)) {
			fclose(file);
			return 1;
		}
		fclose(file);
        printf("tokenized!\n");
		Lexer lexer = lexer_new(&tokens, file_path);
		if (!lexer_parse(&lexer)) {
			return 1;
		}
        printf("lexed!\n");
		fatptr_free(&content);
		vec_free(&tokens);
    } else {
        printf("unknown command!\n");
    }
    return 0;
}
