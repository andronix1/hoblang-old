#pragma once

#include <stdbool.h>
#include <stddef.h>
#include "impl.h"

bool cmd_parse(Cmd *output, const char **args, size_t len);
