#include <stdio.h>
#include <assert.h>
#include <string.h>
#include "tokenizer/tokenizer.h"

const char *args_shift(int *argc, const char ***argv) {
    *argc -= 1;
    const char *cached = (*argv)[0];
    *argv = &(*argv)[1];
    return cached;
}

int main(int argc, const char **argv) {
    args_shift(&argc, &argv);
    const char *command = args_shift(&argc, &argv);
    if (!strcmp(command, "tokens")) {
        File file;
        if (!file_open(&file, "../examples/test.hob", "r")) {
            return 1;
        }
        FileTxReader txr = ftx_new(file);
        Tokenizer t = tokenizer_new(&txr);
        while (token_next(&t)) {
            printf("examples/test.hob:%ld:%ld - token %d\n", txr.location.line, txr.location.character, t.token.type);
        }
        ftx_free(&txr);
        printf("finished!\n");
    } else {
        printf("unknown command!\n");
    }
    return 0;
}
