
#include <iostream>
#include "Record.h"
#include "File.h"
#include "DBFile.h"
#include <stdlib.h>
using namespace std;

extern "C" {
	int yyparse(void);   // defined in y.tab.c
}

extern struct AndList *final;

int main () {

	// try to parse the CNF
	cout << "Enter in your CNF: ";
  	if (yyparse() != 0) {
		cout << "Can't parse your CNF.\n";
		exit (1);
	}
	
	// suck up the schema from the file
	Schema lineitem ("catalog", "lineitem");

	// grow the CNF expression from the parse tree 
	CNF myComparison;
	Record literal;
	myComparison.GrowFromParseTree (final, &lineitem, literal);
	
	// print out the comparison to the screen
	myComparison.Print ();

	// now open up the text file and start procesing it
        //FILE *tableFile = fopen ("lineitem.tbl", "r");

        //Record temp;
        Schema mySchema ("catalog", "lineitem");

	//char *bits = literal.GetBits ();
	//cout << " numbytes in rec " << ((int *) bits)[0] << endl;
	//literal.Print (&supplier);

        // read in all of the records from the text file and see if they match
	// the CNF expression that was typed in
	//int counter = 0;
	//ComparisonEngine comp;
   /*
	while (temp.SuckNextRecord (&mySchema, tableFile) == 1) {
			counter++;
			if (counter % 10000 == 0) {
				cerr << counter << "\n";
			}

			if (comp.Compare (&temp, &literal, &myComparison))
                		temp.Print (&mySchema);

    }
	*/
	/*
	File trialFile;
	char fileName[] = "test";
	trialFile.Open(0,fileName);

	Page trialPage;
	Record trialRecord;
	FILE *trialTableFile = fopen("lineitem.tbl", "r");
	while (trialRecord.SuckNextRecord (&mySchema, trialTableFile) == 1) {
		if (comp.Compare(&trialRecord, &literal, &myComparison))
			trialPage.Append(&trialRecord);
	}
	trialFile.AddPage(&trialPage,1);

	Page readablePage;
	trialFile.GetPage(&readablePage,1);
	Record readableRecord;
	*/

	cerr << "lokesh" << "\n";
	/*while (readablePage.GetFirst(&readableRecord))
	{
		readableRecord.Print(&mySchema);
	}*/

	fType testFileType = fType::heap;
	/*
	DBFile dbFile;
	char testFileName[] = "dbfileTest";
	
	dbFile.Create(testFileName,testFileType,NULL);
	dbFile.Open(testFileName);

	cerr << "dbfile opened" << "\n";
	while (readablePage.GetFirst(&readableRecord))
	{
		dbFile.Add(readableRecord);
		//readableRecord.Print(&mySchema);
	}

	dbFile.Close();
	*/

	/*
	File file;
	file.Open(1, "lineitem.tbl");
	Page p;// = new Page();
	file.GetPage(&p, 0);
	Record rec;
	p.GetFirst(&rec);
	rec.Print(&mySchema);
	p.EmptyItOut();
	p.GetFirst(&rec);
	

	rec.Print(&mySchema);
	file.Close();
	*/

	DBFile dbFile1;
	char testFileName1[] = "dbfileTest1";
	dbFile1.Create(testFileName1, testFileType, NULL);
	//dbFile1.Open(testFileName1);
	dbFile1.Load(mySchema, "lineitem.tbl");
	dbFile1.MoveFirst();
	cout << "after move" << endl;
	dbFile1.Close();
	
	//File file;
	//file.Open(1,testFileName1);
	//Page p;
	//Record rec;
	//cout << file.GetLength() << endl;
	//for (int i = 0; i < file.GetLength(); i++) {
	//	file.GetPage(&p,i);

	//	while (p.GetFirst(&rec)) {
	//		rec.Print(&mySchema);
	//		//cout << endl;
	//	}
	//}
	//file.Close();

	//File file;
	//file.Open(1, "dbfileTest1");
	//Page p;// = new Page();
	//file.GetPage(&p, 0);
	//Record rec;
	//p.GetFirst(&rec);
	//rec.Print(&mySchema);
	////p.EmptyItOut();
	//p.GetFirst(&rec);
	//rec.Print(&mySchema);
	//file.Close();

	// Working until line 148 above without segfaults. Only GetNext remaining.


	cout << "********************* Test GetNext *************************" << endl;

	//DBFile trydbf;
	//trydbf.Open(testFileName1);
	////trydbf.GetNext(first);
	////first.Print(&mySchema);
	//cout << endl;
	//Record first;
	//for (int i = 0; i < 60174; i++) {

	//	trydbf.GetNext(first);
	//	first.Print(&mySchema);
	//	cout << endl;
	//}
	//Record s;
	//trydbf.GetNext(s);
	//s.Print(&mySchema);


	//Test2
	//DBFile trydbf2;
	//Record temp;
	//char testFileName2[] = "dbfileTest2";
	////trydbf2.Open(testFileName2);
	//trydbf2.Create(testFileName2, testFileType, NULL);
	//FILE *fileToRead = fopen("lineitem.tbl", "r");
	//temp.SuckNextRecord(&mySchema, fileToRead);
	//trydbf2.Add(temp);
	//temp.SuckNextRecord(&mySchema, fileToRead);
	//trydbf2.Add(temp); 
	//temp.SuckNextRecord(&mySchema, fileToRead);
	//trydbf2.Add(temp);
	//cout << trydbf2.GetNext(temp) << endl;
	//temp.Print(&mySchema);

	DBFile trydbf;
	trydbf.Open(testFileName1);
	//trydbf.GetNext(first);
	//first.Print(&mySchema);
	cout << endl;
	Record first;
	for (int i = 0; i < 10; i++) {

		trydbf.GetNext(first,myComparison,literal);
		first.Print(&mySchema);
		cout << endl;
	}
	Record s;
	trydbf.GetNext(s);
	s.Print(&mySchema);


	//dbFile1.Close();
}


