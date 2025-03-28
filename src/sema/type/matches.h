#pragma once

#include "core/vec.h"
#include "sema/interface/type.h"
#include "sema/interface/module.h"
#include <malloc.h>

typedef struct {
    SemaType *generic;
    SemaType *match;
} SemaTypeMatch;

static inline SemaTypeMatch sema_type_match_new(SemaType *generic, SemaType *match) {
    SemaTypeMatch result = {
        .generic = generic,
        .match = match,
    };
    return result;
}

typedef struct {
    SemaTypeMatch *matches;
} SemaTypeMatches;

static inline SemaTypeMatches *sema_type_matches_new() {
    SemaTypeMatches *result = malloc(sizeof(SemaTypeMatches));
    result->matches = vec_new(SemaTypeMatch);
    return result;
}

bool sema_type_matches(SemaModule *sema, SemaType *type, SemaType *other, SemaTypeMatches *output);
SemaType *sema_type_generate(SemaType *type, SemaTypeMatches *matches);

