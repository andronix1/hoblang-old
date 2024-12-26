#include "ftx.h"

FileTxReader ftx_new(File file) {
	FileTxReader reader = {
		.reader = file_reader_new(file),
		.location = { 1, 1 },
		.last_location = { 1, 1 },
		.tx = tx_buffer_new()
	};
	return reader;
}

bool ftx_next(FileTxReader *reader) {
	if (tx_buffer_next(&reader->tx)) {
		reader->value = reader->tx.value;
		return true;
	}
	if (!file_reader_next(&reader->reader)) {
		return false;
	}
	reader->location = reader->reader.location;
	tx_buffer_push(&reader->tx, reader->reader.value);
	return ftx_next(reader);
}

bool ftx_first(FileTxReader *reader, bool (*filter)(char)) {
    while (true) {
        if (!ftx_next(reader)) {
            return false;
        }
        if (filter(reader->value)) {
            break;
        }
    }
    return true;
}

void ftx_rollback(FileTxReader *reader) {
	tx_buffer_rollback(&reader->tx);
	reader->value = reader->tx.value;
	reader->location = reader->last_location;
}

void ftx_commit(FileTxReader *reader) {
	tx_buffer_commit(&reader->tx);
	reader->last_location = reader->location;
}

void ftx_free(FileTxReader *reader) {
	tx_buffer_free(&reader->tx);
}
