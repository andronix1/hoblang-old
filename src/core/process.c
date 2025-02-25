#include <sys/wait.h>
#include <unistd.h>
#include "core/process.h"
#include "core/vec.h"

bool process_run(const char *file, char **args, int *status) {
    char **real_args = vec_new(char*);
    real_args = vec_push(real_args, (char**)&file);
    real_args = vec_append_raw(
        real_args,
        args,
        vec_len(args)
    );
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
