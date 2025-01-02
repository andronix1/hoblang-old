#pragma once

#include <stdarg.h>
#include <stdio.h>
#include "location.h"

typedef enum { LOGE, LOGW, LOGI } LogLevel;

void hob_log(LogLevel level, const char *fmt, ...);
void hob_log_at(LogLevel level, const char *path, FileLocation location, const char *fmt, ...);
