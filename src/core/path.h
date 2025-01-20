#pragma once

#include <stdlib.h>
#include <unistd.h>
#include <linux/limits.h>

char *realpath_rel(const char *path, const char *wd);
