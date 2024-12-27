#pragma once

#include <stdbool.h>
#include <string.h>
#include "fatptr.h"

typedef struct {
	size_t idx;
	size_t size;
	FatPtr buffer;
	char value;
} TxReadBuffer;

TxReadBuffer tx_buffer_new();
bool tx_buffer_next(TxReadBuffer *buffer);
void tx_buffer_push(TxReadBuffer *buffer, char c);
void tx_buffer_rollback(TxReadBuffer *buffer);
void tx_buffer_commit(TxReadBuffer *buffer);
void tx_buffer_free(TxReadBuffer *buffer);
