
#include <iostream>
#include "Record.h"
#include "File.h"
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
        FILE *tableFile = fopen ("lineitem.tbl", "r");

        Record temp;
        Schema mySchema ("catalog", "lineitem");

	//char *bits = literal.GetBits ();
	//cout << " numbytes in rec " << ((int *) bits)[0] << endl;
	//literal.Print (&supplier);

        // read in all of the records from the text file and see if they match
	// the CNF expression that was typed in
	int counter = 0;
	ComparisonEngine comp;
    while (temp.SuckNextRecord (&mySchema, tableFile) == 1) {
			counter++;
			if (counter % 10000 == 0) {
				cerr << counter << "\n";
			}

			if (comp.Compare (&temp, &literal, &myComparison))
                		temp.Print (&mySchema);

    }
	
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
	cerr << "lokesh" << "\n";
	while (readablePage.GetFirst(&readableRecord))
	{
		readableRecord.Print(&mySchema);
	}

}


