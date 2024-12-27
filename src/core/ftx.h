#pragma once

#include "file.h"
#include "tx_buffer.h"

typedef struct {
	FileReader reader;
	FileLocation location, last_location;
	TxReadBuffer tx;
	char value;
} FileTxReader;

FileTxReader ftx_new(File file);
bool ftx_next(FileTxReader *reader);
bool ftx_first(FileTxReader *reader, bool (*filter)(char));
bool ftx_skip(FileTxReader *reader, bool (*filter)(char));
void ftx_rollback(FileTxReader *reader);
void ftx_commit(FileTxReader *reader);
void ftx_free(FileTxReader *reader);

