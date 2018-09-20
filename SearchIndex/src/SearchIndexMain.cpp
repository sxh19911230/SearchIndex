/*
 *
 */

#include <iostream>

#include "InvertedIndexADT.h"

using namespace std;

void usage(char* arg);
void unit_test_next(InvertedIndexADT&);

int main(int args, char** argv) try {
	InvertedIndexADT t{"testfile.txt"};
	t.print_inveted_index();
	/* Unit Test next()
	unit_test_next(t);
	*/
} catch (FileNotExist& e) {
	cout << "Unable to open File " << argv[5] << '\n';
	usage(argv[0]);
}

void usage(char* arg) {
	cout << "usage: " << arg << "<file> [cos|proximity] <#output> <query>\n";
}


void unit_test_next(InvertedIndexADT & t) {
	for(int i = 0; i < 400; ++i){
		Term t2 = t.next("the",i/200+1,i %200);
		cout << "the "<<i/200+1<< ' ' <<i %200<<" = " << t2.doc << ' ' << t2.index << '\n';
	}
}
