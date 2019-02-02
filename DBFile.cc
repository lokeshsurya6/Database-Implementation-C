#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <fstream>
#include "Record.h"
#include "Schema.h"
#include "File.h"
#include "HeapFile.h"
#include "RawDBFile.h"
#include "DBFile.h"
#include "Defs.h"

DBFile::DBFile () {

}

DBFile::~DBFile() {

}

int DBFile::Create (const char *f_path, fType f_type, void *startup) {
	char metaFileName[200];
	strcpy(metaFileName,f_path);
	strcat(metaFileName, "-meta.txt");

	ofstream metaFileStream;
	metaFileStream.open(metaFileName);
	if (f_type == heap) {
		metaFileStream << "heap";
		rawDBFile = new HeapFile();
	}

	metaFileStream.close();
	return rawDBFile->Create(f_path,startup);
}

void DBFile::Load (Schema &f_schema, const char *loadpath) {
	rawDBFile->Load(f_schema,loadpath);
}

int DBFile::Open (const char *f_path) {
	char metaFileName[200];
	strcpy(metaFileName, f_path);
	strcat(metaFileName, "-meta.txt");

	ifstream metaFileStream;
	metaFileStream.open(metaFileName);
	string metaContent;
	if (metaFileStream.is_open()) {
		getline(metaFileStream,metaContent);
	}
	else {
		return 0;
	}

	if (metaContent == "heap") {
		rawDBFile = new HeapFile();
	}

	metaFileStream.close();
	return rawDBFile->Open(f_path);
}

void DBFile::MoveFirst () {
	rawDBFile->MoveFirst();
}

int DBFile::Close () {
	return rawDBFile->Close();
}

void DBFile::Add (Record &rec) {
	rawDBFile->Add(rec);
}

int DBFile::GetNext (Record &fetchme) {
	rawDBFile->GetNext(fetchme);
}

int DBFile::GetNext (Record &fetchme, CNF &cnf, Record &literal) {
	rawDBFile->GetNext(fetchme, cnf, literal);
}
