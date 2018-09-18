/*
 *
 */

#include <iostream>

#include "SearchIndex.h"

using namespace std;

void usage(char* arg);

int main(int args, char** argv) {
	unique_ptr<SearchIndex> t;
	try {
		t.reset(new SearchIndex{"testfile.txt"});
	} catch (FileNotExist& e) {
		cout << "Unable to open File " << argv[5] << '\n';
		usage(argv[0]);
	}

	t->print_inveted_index();
}


void usage(char* arg) {
	cout << "usage: " << arg << "<file> [cos|proximity] <#output> <query>\n";
}
