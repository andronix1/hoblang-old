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
	printf("NEXT: ");
	if (tx_buffer_next(&reader->tx)) {
		printf("USING CACHED %c\n", reader->tx.value);
		reader->value = reader->tx.value;
		return true;
	}
	if (!file_reader_next(&reader->reader)) {
		return false;
	}
	printf("READED %c\n", reader->reader.value);
	reader->location = reader->reader.location;
	tx_buffer_push(&reader->tx, reader->reader.value);
	return ftx_next(reader);
}

bool ftx_skip(FileTxReader *reader, bool (*filter)(char)) {
	while (tx_buffer_next(&reader->tx)) {
		if (filter(reader->tx.value)) {
			reader->value = reader->tx.value;
			return true;
		}
	}
	while (file_reader_next(&reader->reader)) {
		if (filter(reader->reader.value)) {
			tx_buffer_push(&reader->tx, reader->reader.value);
			reader->last_location = reader->location = reader->reader.location;
			reader->value = reader->tx.value;
			return true;
		}
	}
    return false;
}

bool ftx_first(FileTxReader *reader, bool (*filter)(char)) {
	ftx_commit(reader);
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
