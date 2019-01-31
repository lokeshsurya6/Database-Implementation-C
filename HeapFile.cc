#include <iostream>
#include <stdlib.h>
#include "Record.h"
#include "Schema.h"
#include "File.h"
#include "DBFile.h"
#include "RawDBFile.h"
#include "HeapFile.h"

HeapFile::HeapFile() {

}

HeapFile::~HeapFile() {

}


int HeapFile::Create(char *f_path, fType f_type, void *startup) {
	mainFile = new File();
	mainFile->Open(0, f_path);
	bufferPage = new Page();
	mainFile->Close();
	return 1;
}

void HeapFile::Load(Schema &f_schema, char *loadpath) {
}

int HeapFile::Open(char *f_path) {
	mainFile->Open(1, f_path);
}

void HeapFile::MoveFirst() {

}

int HeapFile::Close() {
	mainFile->Close();
	return 1;
}

void HeapFile::Add(Record &rec) {
	int result = bufferPage->Append(&rec);
	if (result == 1) {
		return;
	}
	else {
		off_t numOfPages = mainFile->GetLength();
		mainFile->AddPage(bufferPage, numOfPages+1);
		bufferPage->EmptyItOut();
		bufferPage->Append(&rec);
	}
}

int HeapFile::GetNext(Record &fetchme) {
}

int HeapFile::GetNext(Record &fetchme, CNF &cnf, Record &literal) {
}