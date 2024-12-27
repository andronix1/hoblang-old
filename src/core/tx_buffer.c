#include "tx_buffer.h"

TxReadBuffer tx_buffer_new() {
	TxReadBuffer result = {
		.idx = 0,
		.size = 0,
		.buffer = fatptr_empty()
	};
	return result;
}

bool tx_buffer_next(TxReadBuffer *buffer) {
	if (buffer->size == 0 || buffer->idx >= buffer->size) {
		return false;
	}
	printf("(%ld - %ld)", buffer->idx, buffer->size);
	buffer->value = buffer->buffer.str_ptr[buffer->idx++];
	return true;
}

void tx_buffer_push(TxReadBuffer *buffer, char c) {
	assert(buffer->buffer.size <= buffer->size);
	buffer->size++;
	if (buffer->buffer.size < buffer->size) {
		fatptr_alloc_more(&buffer->buffer, 1);
	}
	buffer->buffer.str_ptr[buffer->size - 1] = c;
}

void tx_buffer_rollback(TxReadBuffer *buffer) {
	printf("RB: %ld[%ld] - '%c'!\n", buffer->size, buffer->idx, buffer->value);
	if (buffer->size == 0) {
		return;
	}
	buffer->idx = 0;
	buffer->value = buffer->buffer.str_ptr[0];
}

void tx_buffer_commit(TxReadBuffer *buffer) {
	buffer->size -= buffer->idx + 1;
	if (buffer->size > 0) {
		memcpy(buffer->buffer.raw_ptr, &buffer->buffer.str_ptr[buffer->idx + 1], buffer->size);
		buffer->value = buffer->buffer.str_ptr[0];
	}
	buffer->idx = 0;
}

void tx_buffer_free(TxReadBuffer *buffer) {
	fatptr_free(&buffer->buffer);
}
