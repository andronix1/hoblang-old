#pragma once

#include "core/print.h"
#include "core/location.h"

typedef enum { LOGF, LOGE, LOGW, LOGI, LOGD } LogLevel;

void hob_log(LogLevel level, const char *fmt, ...);
void hob_log_at(LogLevel level, const char *path, FileLocation location, const char *fmt, ...);
