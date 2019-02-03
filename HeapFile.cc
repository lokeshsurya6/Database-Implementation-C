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


int HeapFile::Create(const char *f_path, void *startup) {
	mainFile = new File();
	mainFile->Open(0, (char *)f_path);
	bufferPage = new Page();
	readPagePointer = new Page();
	currentPageNumber = 0;
	isBufferEmpty = 1;
	return 1;
}

int HeapFile::Open(const char *f_path) {
	mainFile = new File();
	mainFile->Open(1, (char *)f_path);
	bufferPage = new Page();
	readPagePointer = new Page();
	if (mainFile->GetLength() > 0) {
		mainFile->GetPage(readPagePointer, 0);
	}
	currentPageNumber = 0;
	isBufferEmpty = 1;
	return 1;
}

void HeapFile::Load(Schema &f_schema, const char *loadpath) {
	FILE *fileToRead = fopen(loadpath, "r");
	
	Record tempRecord;
	while (tempRecord.SuckNextRecord(&f_schema, fileToRead) == 1) {
		Add(tempRecord);
	}

	if (isBufferEmpty == 0) {
		int totalPages = mainFile->GetLength();
		totalPages = (totalPages == 0) ? 0 : totalPages - 1;
		mainFile->AddPage(bufferPage, totalPages);
		bufferPage->EmptyItOut();
		isBufferEmpty = 1;
	}

	//cout << "Load done. Number of Pages: " << mainFile->GetLength() << endl;
	fclose(fileToRead);
}

void HeapFile::MoveFirst() {
	off_t totalNumOfPages = mainFile->GetLength();
	if (totalNumOfPages == 0) {
		if (isBufferEmpty == 0) {
			mainFile->AddPage(bufferPage, 0);
			bufferPage->EmptyItOut();
			isBufferEmpty = 1;
		}
		else {
			readPagePointer = new Page();
			currentPageNumber = 0;
			return;
		}
	}

	mainFile->GetPage(readPagePointer, 0);
	currentPageNumber = 0;
}

void HeapFile::Add(Record &rec) {
	int result = bufferPage->Append(&rec);

	if (result == 1) {
		isBufferEmpty = 0;
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
		isBufferEmpty = 1;
		bufferPage->Append(&rec);
		isBufferEmpty = 0;
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
		if (isBufferEmpty == 0) {
			mainFile->AddPage(bufferPage, 0);
			bufferPage->EmptyItOut();
			isBufferEmpty = 1;

			mainFile->GetPage(readPagePointer, 0);
			currentPageNumber = 0;

			return readPagePointer->GetFirst(&fetchme);
		}
	}
	// Case2 - Part B : Pages available in disk
	else {
		currentPageNumber++;

		// Case2 - Part B - SubPart 1: totalPagesInDisk-1 > currentPageNumber
		if (totalPagesInDisk - 1 > currentPageNumber) {
			mainFile->GetPage(readPagePointer,currentPageNumber);
			return readPagePointer->GetFirst(&fetchme);
		} 
		// Case2 - Part B - SubPart 2: totalPagesInDisk -1 == currentPageNumber
		else if (totalPagesInDisk - 1 == currentPageNumber) {
			if (isBufferEmpty == 0) {
				mainFile->AddPage(bufferPage, currentPageNumber);
				bufferPage->EmptyItOut();
				isBufferEmpty = 1;

				mainFile->GetPage(readPagePointer, currentPageNumber);
				return readPagePointer->GetFirst(&fetchme);
			}
		}
	}
	return 0;

}

int HeapFile::GetNext(Record &fetchme, CNF &cnf, Record &literal) {
	ComparisonEngine comp;
	Page *temp = new Page();
	temp = readPagePointer;
	int tempPageNumber = currentPageNumber;

	while (GetNext(fetchme)) {
		if (comp.Compare(&fetchme, &literal, &cnf)) {
			return 1;
		}
		else {
			continue;
		}
	}

	readPagePointer = temp;
	currentPageNumber = tempPageNumber;
	temp = NULL;
	return 0;
}

int HeapFile::Close() {
	if (isBufferEmpty == 0) {
		int totalPages = mainFile->GetLength();
		totalPages = (totalPages == 0) ? 0 : totalPages - 1;
		mainFile->AddPage(bufferPage, totalPages);
		bufferPage->EmptyItOut();
		isBufferEmpty = 1;
	}
	mainFile->Close();
	delete mainFile;
	mainFile = NULL;
	delete bufferPage;
	bufferPage = NULL;
	readPagePointer = NULL;
	isBufferEmpty = 1;
	currentPageNumber = 0;
	return 1;
}