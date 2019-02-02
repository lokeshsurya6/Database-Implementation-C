#ifndef HEAPFILE_H
#define HEAPFILE_H

#include "Record.h"
#include "Schema.h"
#include "File.h"
#include "RawDBFile.h"

class HeapFile : public RawDBFile {
private:
	File *mainFile;
	Page *bufferPage;
	Record *recordPointer;
	Page *readPagePointer;
	int currentPageNumber;
	int isBufferEmpty;

public:
	HeapFile();
	~HeapFile();
	int Create(const char *fpath, void *startup);
	int Open(const char *fpath);
	int Close();

	void Load(Schema &myschema, const char *loadpath);

	void MoveFirst();
	void Add(Record &addme);
	int GetNext(Record &fetchme);
	int GetNext(Record &fetchme, CNF &cnf, Record &literal);
};
#endif
