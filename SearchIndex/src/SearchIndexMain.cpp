/*
 *
 */

#include <iostream>

#include "InvertedIndexADT.h"

using namespace std;

void usage(char* arg);

int main(int args, char** argv) try {
	InvertedIndexADT t{"testfile.txt"};
	t.print_inveted_index();
} catch (FileNotExist& e) {
	cout << "Unable to open File " << argv[5] << '\n';
	usage(argv[0]);
}





void usage(char* arg) {
	cout << "usage: " << arg << "<file> [cos|proximity] <#output> <query>\n";
}
