#pragma once

#define NOT_NULL(expr) ({ typeof(expr) _val = expr; if (!_val) return NULL; _val; })
#define RET_ON_NULL(expr) ({ typeof(expr) _val = expr; if (!_val) return; _val; })
#define RET_IF_OK(expr) do { \
    typeof(expr) _val = expr; \
    if (_val) return _val; \
} while(0)
