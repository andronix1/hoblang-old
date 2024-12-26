#include "tx_buffer.h"

TxReadBuffer tx_buffer_new() {
	TxReadBuffer result = {
		.idx = -1,
		.size = 0,
		.buffer = fatptr_empty()
	};
	return result;
}

bool tx_buffer_next(TxReadBuffer *buffer) {
	if (buffer->size == 0 || buffer->idx >= buffer->size) {
		return false;
	}
	buffer->idx++;
	buffer->value = buffer->buffer.str_ptr[buffer->idx];
	printf("%d - '%c'\n", buffer->size, buffer->value);
	return true;
}

void tx_buffer_push(TxReadBuffer *buffer, char c) {
	if ((size_t)buffer->size >= buffer->buffer.size) {
		fatptr_alloc_more(&buffer->buffer, 1);
	}
	buffer->buffer.str_ptr[buffer->size] = c;
	buffer->size += 1;
}

void tx_buffer_rollback(TxReadBuffer *buffer) {
	buffer->idx = 0;
	buffer->value = buffer->buffer.str_ptr[0];
}

void tx_buffer_free(TxReadBuffer *buffer) {
	fatptr_free(&buffer->buffer);
}

void tx_buffer_commit(TxReadBuffer *buffer) {
	assert(buffer->idx < buffer->size);
	buffer->size -= buffer->idx + 1;
	if (buffer->size > 0) {
		memcpy(buffer->buffer.raw_ptr, &buffer->buffer.str_ptr[buffer->idx], buffer->size);
	}
	buffer->idx = -1;
}
