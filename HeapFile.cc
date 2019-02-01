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
	return 1;
}

void HeapFile::Load(Schema &f_schema, char *loadpath) {
	FILE *fileToRead = fopen(loadpath, "r");
	Record tempRecord;
	
	while (tempRecord.SuckNextRecord(&f_schema, fileToRead) == 1) {
		Add(tempRecord);
	}

	int totalPages = mainFile->GetLength();
	totalPages = (totalPages == 0) ? 0 : totalPages - 1;
	mainFile->AddPage(bufferPage, totalPages);

	cout << "Load done. Number of Pages: " << mainFile->GetLength() << endl;
}

int HeapFile::Open(char *f_path) {
	mainFile = new File();
	mainFile->Open(1, f_path);
	bufferPage = new Page();
	return 1;
}

void HeapFile::MoveFirst() {
	off_t totalNumOfPages = mainFile->GetLength();
	if (totalNumOfPages == 0) {
		mainFile->AddPage(bufferPage, 0);
		bufferPage->EmptyItOut();
	}

	Page temp;
	mainFile->GetPage(&temp, 0);
	readPagePointer = &temp;
	
	Record rec;
	temp.GetFirst(&rec);
	recordPointer = &rec;
	currentPageNumber = 0;
}

int HeapFile::Close() {
	mainFile->Close();
	mainFile = NULL;
	bufferPage = NULL;
	return 1;
}

void HeapFile::Add(Record &rec) {
	Record temp = rec;
	int result = bufferPage->Append(&rec);

	if (result == 1) {
		return;
	}
	else {
		off_t totalPages = mainFile->GetLength();
		totalPages = (totalPages == 0) ? 0 : totalPages - 1;
		mainFile->AddPage(bufferPage, totalPages);
		//bufferPage->EmptyItOut();
		//delete bufferPage;
		//bufferPage = NULL;
		bufferPage = new Page();
		bufferPage->Append(&temp);
	}
}

int HeapFile::GetNext(Record &fetchme) {

}

int HeapFile::GetNext(Record &fetchme, CNF &cnf, Record &literal) {
}