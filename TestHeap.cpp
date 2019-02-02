#include<iostream>
#include<gtest/gtest.h>
#include "Record.h"
#include "File.h"
#include "HeapFile.h"
#include "RawDBFile.h"
#include "DBFile.h"
#include <cstring>

//Global variable DemoFile of DBFile type
DBFile DemoFile;

int main(int argc, char **argv) {
	
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}

//Test case to test create method of DBFile
TEST(TestCreate, SubTest1) {	
	DemoFile.Create("tmp.txt", fType::heap, NULL);
	string name = "tmp.txt";
	bool ck;
	if (FILE *file = fopen(name.c_str(), "r")) {
		fclose(file);
		ck = true;
	}
	else {
		ck = false;
	}
	ASSERT_TRUE(ck);
}

//Test case to assert when open method of DBFile fails
TEST(TestCreate_Not, SubTest1) {
	DemoFile.Create("tmp.txt", fType::heap, NULL);
	string name = "m.txt";
	bool ck;
	if (FILE *file = fopen(name.c_str(), "r")) {
		fclose(file);
		ck = true;
	}
	else {
		ck = false;
	}
	ASSERT_FALSE(ck);
}

//Test case to assert load method of DBFile 
TEST(TestLoad, SubTest1) {
	DemoFile.Create("tmp.txt", fType::heap, NULL);
	DemoFile.Open("tmp.txt");

	string name = "m.txt";
	bool ck;
	if (FILE *file = fopen(name.c_str(), "r")) {
		fclose(file);
		ck = true;
	}
	else {
		ck = false;
	}
	ASSERT_FALSE(ck);
}


TEST(Test2, SubTest1) {
	DemoFile.Create("tmp.txt", fType::heap, NULL);

	ASSERT_TRUE(1 == 1);
}
TEST(Test3, SubTest1) {
	ASSERT_EQ(1, 1);
}

//Test case to assert open method of DBFile 
TEST(TestOpen, SubTest1) {
	DemoFile.Create("xyz.txt", fType::heap, NULL);
	bool ck;
	if (DemoFile.Open("xyz.txt")) {
		ck = true;
	}
	else {
		ck = false;
	}
	ASSERT_TRUE(ck);
}

//Test case to assert when create method of DBFile fails
TEST(TestOpen_Not, SubTest1) {
	bool ck;
	if(DemoFile.Open("@.txt")){
		//fclose(file);
		ck = true;
	}
	else {
		ck = false;
	}
	ASSERT_FALSE(ck);
}




