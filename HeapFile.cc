#include <iostream>
#include <stdlib.h>
#include "Record.h"
#include "Schema.h"
#include "File.h"
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
	readPagePointer = new Page();
	currentPageNumber = 0;
	return 1;
}

int HeapFile::Open(char *f_path) {
	mainFile = new File();
	mainFile->Open(1, f_path);
	bufferPage = new Page();
	readPagePointer = new Page();
	mainFile->GetPage(readPagePointer,0);
	currentPageNumber = 0;
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
	fclose(fileToRead);
}

void HeapFile::MoveFirst() {
	off_t totalNumOfPages = mainFile->GetLength();
	if (totalNumOfPages == 0) {
		mainFile->AddPage(bufferPage, 0);
		bufferPage->EmptyItOut();
	}

	mainFile->GetPage(readPagePointer, 0);
	
	//Record rec;
	//temp.GetFirst(&rec);
	//recordPointer = &rec;
	currentPageNumber = 0;

	//TODO
	// Remove test code
	/*
	Schema mySchema("catalog", "lineitem");
	(rec).Print(&mySchema);
	cout << endl;
	temp.GetFirst(&rec);
	(rec).Print(&mySchema);
	cout << endl;

	Record rec1;
	temp.GetFirst(&rec1);
	(rec1).Print(&mySchema);
	cout << endl;
	*/
}

void HeapFile::Add(Record &rec) {
	int result = bufferPage->Append(&rec);

	if (result == 1) {
		return;
	}
	else {
		off_t totalPages = mainFile->GetLength();
		totalPages = (totalPages == 0) ? 0 : totalPages - 1;
		mainFile->AddPage(bufferPage, totalPages);
		if (totalPages == 0) {
			// Update readPagePointer
			mainFile->GetPage(readPagePointer,0);
			currentPageNumber = 0;
		}
		bufferPage->EmptyItOut();
		//delete bufferPage;
		//bufferPage = NULL;
		//bufferPage = new Page();
		bufferPage->Append(&rec);
	}
}

int HeapFile::GetNext(Record &fetchme) {
	// Case1 : Data available in readPagePointer
	int result = readPagePointer->GetFirst(&fetchme);
	if(result == 1) {
		return 1;
	}

	//Case2 : Data NOT available in readPagePointer
	off_t totalPagesInDisk = mainFile->GetLength();
	// Case2 - Part A : No pages in Disk
	if (totalPagesInDisk == 0) {
		mainFile->AddPage(bufferPage, 0);
		bufferPage->EmptyItOut();
		
		mainFile->GetPage(readPagePointer, 0);
		currentPageNumber = 0;

		int result = readPagePointer->GetFirst(&fetchme);
		if (result == 1) {
			return 1;
		}
		else {
			return 0;
		}
	}
	// Case2 - Part B : Pages available in disk
	else {
		currentPageNumber++;

		// Case2 - Part B - SubPart 1: totalPagesInDisk-1 > currentPageNumber
		if (totalPagesInDisk - 1 > currentPageNumber) {
			mainFile->GetPage(readPagePointer,currentPageNumber);
			readPagePointer->GetFirst(&fetchme);
			return 1;
		} 
		// Case2 - Part B - SubPart 2: totalPagesInDisk -1 == currentPageNumber
		else if (totalPagesInDisk - 1 == currentPageNumber) {
			mainFile->AddPage(bufferPage, currentPageNumber);
			bufferPage->EmptyItOut();

			mainFile->GetPage(readPagePointer, currentPageNumber);

			int result = readPagePointer->GetFirst(&fetchme);
			if (result == 1) {
				return 1;
			}
			else {
				return 0;
			}
		}
	}
	// Worst Case
	return 0;

}

int HeapFile::GetNext(Record &fetchme, CNF &cnf, Record &literal) {
	ComparisonEngine comp;
	Page temp;
	temp = (*readPagePointer);
	int tempPageNumber = currentPageNumber;

	while (GetNext(fetchme)) {
		if (comp.Compare(&fetchme, &literal, &cnf)) {
			return 1;
		}
		else {
			continue;
		}
	}

	*readPagePointer = temp;
	currentPageNumber = tempPageNumber;

	return 0;
}

int HeapFile::Close() {
	//TODO
	// Check if working
	int totalPages = mainFile->GetLength();
	totalPages = (totalPages == 0) ? 0 : totalPages - 1;
	mainFile->AddPage(bufferPage, totalPages);

	mainFile->Close();
	delete mainFile;
	mainFile = NULL;
	delete bufferPage;
	bufferPage = NULL;
	readPagePointer = NULL;
	currentPageNumber = 0;
	return 1;
}