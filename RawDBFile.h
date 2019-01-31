#ifndef RAWDBFILE_H
#define RAWDBFILE_H

#include "Record.h"
#include "Schema.h"
#include "File.h"

typedef enum { heap, sorted, tree } fType;

class RawDBFile {
public:
	RawDBFile();
	~RawDBFile();

	virtual int Create(char *fpath, fType file_type, void *startup);
	virtual int Open(char *fpath);
	virtual int Close();

	virtual void Load(Schema &myschema, char *loadpath);

	virtual void MoveFirst();
	virtual void Add(Record &addme);
	virtual int GetNext(Record &fetchme);
	virtual int GetNext(Record &fetchme, CNF &cnf, Record &literal);

};
#endif
