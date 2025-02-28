#include <sys/wait.h>
#include <unistd.h>
#include "core/process.h"
#include "core/vec.h"

bool process_run(const char *file, char **args, int *status) {
    char **real_args = vec_new(char*);
    real_args = vec_push(real_args, (char**)&file);
    for (size_t i = 0; i < vec_len(args); i++) {
        real_args = vec_push(real_args, &args[i]);
    }
    char *end = NULL;
    real_args = vec_push(real_args, &end);
    pid_t pid = vfork();
    if (pid == 0) {
        if (execv(file, real_args) < 0) {
            return false;
        }
        return false;
    }
    waitpid(pid, status, 0);
    return true;
}
