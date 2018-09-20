/*
 * SearchIndex.cpp
 *
 *  Created on: 2018Äê9ÔÂ18ÈÕ
 *      Author: Forrest
 */

#include <fstream>
#include <sstream>
#include <algorithm>
#include <iostream>

#include "InvertedIndexADT.h"
using namespace std;

//inline vector returned when result not found
static const Term& unfounded{-1,-1};

InvertedIndexADT::InvertedIndexADT(std::string filename) {
	this->init_inveted_index(filename);
}

void InvertedIndexADT::print_inveted_index() {
	for(auto i = inverted_index.begin();i != inverted_index.end();++i){
	    cout << i->first << '\n' << i->second.document_occurence.size() <<','<<i->second.terms.size() << '\n';
	    for (auto j = i->second.document_occurence.begin();j!= i->second.document_occurence.end();j++)
	    	cout << '(' <<j->first << ',' << j->second << ") ";
	    cout << '\n';
	    for (auto j = i->second.terms.begin();j != i->second.terms.end();++j)
	        cout << '(' << j->doc << ',' << j->index << ") ";
	    cout << '\n';
	}
}

//Galloping Search
const Term& InvertedIndexADT::next(const std::string& term, int doc_num, int ind_num) {
	const std::vector<Term>& P = inverted_index[term].terms;
	const Term current {doc_num,ind_num};

	if (P.size()==0 || P.back() <= current)
		return unfounded;
	if (P[0] > current){
			c[term]=0;
			return P[c[term]];
	}
	size_t low = 0;
	if (c[term] > 0 && P[c[term]-1] <= current)
		low = c[term]-1;
	else
		low=0;

	int jump=1;
	size_t high = low+jump;

	while(high < P.size() && P[high] <= current) {
		low=high;
		jump=2*jump;
		high=low+jump;
	}
	if (high > P.size()) high = P.size();
	c[term] = binarySearch(term,low,high,current);
	return P[c[term]];
}


const Term& InvertedIndexADT::prev(const std::string& term, int doc_num, int current) {

	return unfounded;
}


void InvertedIndexADT::init_inveted_index(std::string filename) {
	ifstream file(filename);
	if (!file) {
		throw FileNotExist{};
	}
	int indexNum=0;
	int document_num = 1;
	for (string line; getline(file,line);++indexNum) {
	        //New Doc if \n\n, reset indexNum
	    if (!line.size()) {
	        ++document_num;
	        indexNum=-1;
	    } else {
	    	transform (begin(line),end(line),begin(line), [](char c){if (!isalnum(c)) c=' '; return ::tolower(c);});
            istringstream str(line);
            for (string term; str >> term;) {
            	++inverted_index[term].document_occurence[document_num];
            	inverted_index[term].terms.push_back(Term{document_num, ++indexNum});
            }
        }
	}
	file.close();
}

int InvertedIndexADT::binarySearch(const string& t,int low, int high, const Term& current) {
	int i = (low + high) / 2;
	while(true){
		if (current==inverted_index[t].terms[i]) return i+1;
		else if (current<inverted_index[t].terms[i]) {
			if(high == i) return i;
			high = i;
			i=(i+low)/2;
		}
		else if (inverted_index[t].terms[i] < current){
			if (low == i) return i+1;
			low = i;
			i=(i+high)/2;
		}
	}
}

bool operator<(const Term& a,const Term& b) {
	return a.doc < b.doc ||(a.doc == b.doc && a.index < b.index);
}

bool operator>(const Term& a,const Term& b) {
	return a.doc > b.doc ||(a.doc == b.doc && a.index > b.index);
}

inline bool operator<=(const Term&a,const Term&b) {
	return !(a>b);
}
inline bool operator>=(const Term&a,const Term&b) {
	return !(a<b);
}
bool operator==(const Term&a,const Term&b) {
	return a.doc == b.doc && a.index == b.index;
}


