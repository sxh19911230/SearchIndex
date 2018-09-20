/*
 *
 */

#include <iostream>

#include "InvertedIndexADT.h"

using namespace std;

void usage(char* arg);
void unit_test_next(InvertedIndexADT&);
void unit_test_prev(InvertedIndexADT&);
void unit_test_consine(InvertedIndexADT& t);

int main(int args, char** argv) try {
	//if (args != 5) {usage(argv[0]);return 0;}

	InvertedIndexADT t{"testfile.txt"};
	//t.print_inveted_index();
	/* Unit Test
	unit_test_next(t);
	unit_test_prev(t);
	//unit_test_next(t);
	//*/
	unit_test_consine(t);


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
		cout << "the "<<i/200+1<< ' ' <<i %200<<" = " << t2.doc << ' ' << t2.index << endl;
	}
}

void unit_test_prev(InvertedIndexADT& t) {
	for(int i = 0; i < 400; ++i){
			Term t2 = t.prev("the",i/200+1,i %200);
			cout << "the "<<i/200+1<< ' ' <<i %200<<" = " << t2.doc << ' ' << t2.index << endl;
		}
}

void unit_test_consine(InvertedIndexADT& t) {
	vector<string> str{"the","ghost","in","the","shell","usa"};
		auto p = t.rankCosine(str);
		cout << "ID  Score" << '\n';
		for (auto q : p)
			cout << q.second << ' ' << q.first << '\n';
}
