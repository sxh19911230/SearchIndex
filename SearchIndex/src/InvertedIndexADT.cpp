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

const Term& InvertedIndexADT::next(std::string term, int doc_num, int current) {
	const std::vector<Term>& term_array = inverted_index[term].terms;


	if (term_array.size()==0 ||
			(term_array.back().doc <= doc_num &&
					term_array.back().index <= current))
		return unfounded;
	if (term_array[0].doc > doc_num || (term_array[0].doc == doc_num && term_array[0].index > current)){
			list_index_positions[term]=0;
			return term_array[list_index_positions[term]];
	}
	//TODO

	return unfounded;

}


const Term& InvertedIndexADT::prev(std::string term, int doc_num, int current) {

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
