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
#include <iterator>
#include <climits>

#include "InvertedIndexADT.h"
using namespace std;

//inline vector returned when result not found
static const Term& infinity{INT_MAX,INT_MAX};
static const Term& ninfinity{INT_MIN,INT_MIN};

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
		return infinity;
	if (P[0] > current){
			c[term]=0;
			return P[c[term]];
	}
	int low = 0;
	if (c[term] > 0 && P[c[term]-1] <= current)
		low = c[term]-1;
	else
		low=0;

	int jump=1;
	int high = low+jump;

	while(high < (int)P.size() && P[high] <= current) {
		low=high;
		jump=2*jump;
		high=low+jump;
	}
	if (high > (int)P.size()-1) high = P.size()-1;
	c[term] = binarySearch(term,low,high,current);
	return P[c[term]];
}


const Term& InvertedIndexADT::prev(const std::string& term, int doc_num, int ind_num) {
	const std::vector<Term>& P = inverted_index[term].terms;
		const Term current {doc_num,ind_num};

		if (P.size()==0 || P.front() >= current)
			return ninfinity;
		if (P.back() < current){
			c[term]=P.size()-1;
			return P[c[term]];
		}
		int high= (c[term] < P.size() -1 && P[c[term]+1] >= current) ?
			c[term]+1 :	P.size()-1;

		int jump=1;
		int low = high-jump;

		while(low > 0 && P[low] >= current) {
			high=low;
			jump=2*jump;
			low=high-jump;
		}
		if (low < 0) low = 0;
		c[term] = binarySearch(term,low,high,current,false);
		return P[c[term]];
}

void InvertedIndexADT::init_inveted_index(std::string filename) {
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

#ifndef DIVBYSPC
	    	/* Default:
	    	 * term divided by any punctuation
	    	 * e.g. U.S.A -> u, s, and a
	    	 */
	    	transform (begin(line),end(line),begin(line), [](char c){if (!isalnum(c)) c=' '; return ::tolower(c);});
#else
	    	// term divided by space and shrimp the punctuation
	    	// e.g. U.S.A. -> usa
	    	string tmp;
	    	transform (begin(line),end(line),begin(line), ::tolower);
	    	copy_if(begin(line),end(line),back_inserter(tmp),[](char c){return ::isalnum(c) || c==' ';});
	    	line=tmp;
#endif

            istringstream str(line);
            for (string term; str >> term;) {
            	++inverted_index[term].document_occurence[document_num];
            	inverted_index[term].terms.push_back(Term{document_num, ++indexNum});
            }
        }
	}
	file.close();
}

std::multimap<double, int,greater<double>> InvertedIndexADT::rankCosine(std::vector<std::string>& terms) {
	auto doc_termNum_mapping = std::map<int, int>{};
	pair<Term,Term> nc = nextCover(terms, 0,0);
	Term u = nc.first;
	Term v = nc.second;
	int d = u.doc;


	while(u < infinity) {
		++doc_termNum_mapping[u.doc];
		u = nextCover(terms, 0,0).first;
	}

	auto p = std::multimap<double, int, greater<double>>{};
	for (auto i = doc_termNum_mapping.begin(); i != doc_termNum_mapping.end();++i){
		double score = log2((double)document_num/doc_termNum_mapping.size())*log2(i->second);
		p.insert(pair<double,int>(score,i->first));
	}
	return p;
}

std::multimap<double, int, std::greater<double> > InvertedIndexADT::rankProximity(std::vector<std::string>& terms) {

	auto p = std::multimap<double, int, greater<double>>{};
	pair<Term,Term> nc = nextCover(terms, 0,0);
	Term u = nc.first;
	Term v = nc.second;
	int d = u.doc;
	double score = 0.0;

	while(u < infinity) {
		if (d<u.doc) {

			p.insert(pair<double,int>{score,d});
			d=u.doc;
			score=0.0;
		}
		score=score+1.0/(v.index-u.index+1);
		nc = nextCover(terms, u.doc,u.index);
		u=nc.first;
		v=nc.second;
	}

	if (d < INT_MAX) {
		p.insert(pair<double,int>{score,d});
	}

	return p;
}

int InvertedIndexADT::binarySearch(const string& t,int low, int high, const Term& current, bool return_higher) {
	int i = (low + high) / 2;
	while(true){
		if (current==inverted_index[t].terms[i]) return i+(return_higher?1:-1);
		else if (current<inverted_index[t].terms[i]) {
			if(high == i) return i+(return_higher?0:-1);
			high = i;
			i=(i+low)/2;
		}
		else if (inverted_index[t].terms[i] < current){
			if (low == i) return i+(return_higher?1:0);
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

pair<Term, Term> InvertedIndexADT::nextCover(const std::vector<std::string>& terms, int doc_num, int ind_num) {
	set<Term,greater<Term>> result{};
	for(const string& s : terms)
		result.insert(next(s,doc_num,ind_num));
	Term v= *result.begin();
	if (v == infinity) return pair<Term,Term>{infinity,infinity};
	Term u = *result.rbegin();
	if(v.doc == u.doc) return pair<Term,Term>{{u.doc,u.index},{v.doc,v.index}};
	else return nextCover(terms, u.doc, u.index);

}
