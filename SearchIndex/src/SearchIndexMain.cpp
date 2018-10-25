/** Authors: Xianghong Sun, Harita Shroff
 *  Date: 09/25/2018
 *  usage: SearchIndex <file> [cos|proximity] <#output> <query>
 *
 *  This program will stdout (#output) of document ID, score in Descending Order 
 *  for each document that has all <query> terms in <file>. The ranking method can
 *  be one of [cos|proximity]. 
 *
 */

#include <iostream>
#include <sstream>
#include <algorithm>
#include <climits>

#include "InvertedIndexADT.h"

using namespace std;

void usage(char* arg);
void unit_test_next(InvertedIndexADT&);
void unit_test_prev(InvertedIndexADT&);
void unit_test_consine(InvertedIndexADT& t);
void unit_test_next_cover(InvertedIndexADT& t);
void unit_test_Proximity(InvertedIndexADT& t);
void unit_test_nextPage(InvertedIndexADT& t);
void unit_test_prePage(InvertedIndexADT& t);

int main(int args, char** argv) try {


	if (args != 5) {usage(argv[0]);return 0;}

	istringstream is {argv[4]};
	vector<string> terms;
	while (is) {
		string t;
		is >> t;
		transform(t.begin(),t.end(),t.begin(),::tolower);
		if (t!="")terms.push_back(t);
	}

	int k = atoi(argv[3]);
	InvertedIndexADT t{argv[1]};
/*
	std::multimap<double, int,greater<double>> r;
	if (string{argv[2]} == string{"cos"})
		r=t.rankCosine(terms);
	else if (string{argv[2]} == string{"proximity"})
		r=t.rankProximity(terms);
	else exit(0);
	int j = 0;
	cout << "ID  Score" << '\n';
	for(auto i = r.begin() ; i != r.end() && j < k;++i,++j)
		cout << i->second << ' ' << i->first << '\n';
*/

	//unit_test_prePage(t);

	auto result = t.rankBM25_TermAtATimeWithPruning(terms);

	for (auto tmp : result) {
		cout << tmp.second << ':' << tmp.first << endl;
	}

} catch (FileNotExist& e) {
	cout << "Unable to open File " << argv[5] << '\n';
	usage(argv[0]);
}

void usage(char* arg) {
	cout << "usage: " << arg << " <file> [cos|proximity] <#output> <query>\n";
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

void unit_test_next_cover(InvertedIndexADT& t) {
	vector<string> str{"the","in"};
	pair<Term,Term> v{{0,0},{0,0}};
	while(true) {
		v = t.nextCover(str,v.first.doc,v.first.index);
		if (v.first==Term{INT_MAX,INT_MAX}) break;
		cout << v.first.doc << ' ' << v.first.index << ' ' << v.second.doc << ' '<< v.second.index << '\n';
	}
}

void unit_test_Proximity(InvertedIndexADT& t) {
	vector<string> str{"the","in"};
		auto p = t.rankProximity(str);
		cout << "ID  Score" << '\n';
		for (auto q : p)
			cout << q.second << ' ' << q.first << '\n';
}

void unit_test_nextPage(InvertedIndexADT& t) {

	string str = "the";
	auto p = 0;
	cout << "Next Page:" << '\n';
	while(p != INT_MAX)
		cout << (p = t.nextDoc(str,p)) << '\n';
}

void unit_test_prePage(InvertedIndexADT& t) {
	string str = "the";
	auto p = 100000;
	cout << "Prev Page:" << '\n';
	while(p != INT_MIN)
		cout << (p = t.prevDoc(str,p)) << '\n';
}
