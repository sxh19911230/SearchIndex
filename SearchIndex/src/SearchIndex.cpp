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
#include "SearchIndex.h"
using namespace std;

SearchIndex::SearchIndex(std::string filename) {
	this->init_inveted_index(filename);
}

void SearchIndex::print_inveted_index() {
	for(auto i = inverted_index.begin();i != inverted_index.end();++i){
	    cout << i->first << '\n' << get<0>(i->second).size() <<','<<get<1>(i->second).size();
	    for (auto j = get<1>(i->second).begin();j != get<1>(i->second).end();++j)
	        cout << ",(" << (*j)[0] << ',' << (*j)[1] << ')';
	    cout <<'\n';
	    }
}

void SearchIndex::init_inveted_index(std::string filename) {
	ifstream file(filename);
	if (!file) {
		throw FileNotExist{};
	}
	int indexNum=0;
	document_num = 1;
	for (string line; getline(file,line);++indexNum) {
	        //New Doc if \n\n, reset indexNum
	    if (!line.size()) {
	        ++document_num;
	        indexNum=-1;
	    } else {
	    	transform (begin(line),end(line),begin(line), [](char c){if (!isalnum(c)) c=' '; return ::tolower(c);});
            istringstream str(line);
            for (string term; str >> term;) {
            	get<0>(inverted_index[term]).insert(document_num);
            	get<1>(inverted_index[term]).emplace_back(new int[2] {document_num, ++indexNum});
            }
        }
	}
	file.close();
}
