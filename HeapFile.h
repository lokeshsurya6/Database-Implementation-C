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

public:
	HeapFile();
	~HeapFile();
	int Create(char *fpath, fType file_type, void *startup);
	int Open(char *fpath);
	int Close();

	void Load(Schema &myschema, char *loadpath);

	void MoveFirst();
	void Add(Record &addme);
	int GetNext(Record &fetchme);
	int GetNext(Record &fetchme, CNF &cnf, Record &literal);
};
#endif
