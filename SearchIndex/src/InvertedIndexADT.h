/*
 * SearchIndex.h
 *
 *  Created on: 2018Äê9ÔÂ18ÈÕ
 *      Author: Forrest
 */

#ifndef INVERTEDINDEXADT_H_
#define INVERTEDINDEXADT_H_
#include <string>
#include <map>
#include <tuple>
#include <vector>
#include <memory>
#include <set>

struct Term {
	int doc;
	int index;
};

struct TermList {
	std::map<int,int> document_occurence;
	//list(document#, index#)
	std::vector<Term> terms;
};

bool operator<(const Term&,const Term&);
bool operator>(const Term&,const Term&);
bool operator<=(const Term&,const Term&);
bool operator>=(const Term&,const Term&);
bool operator==(const Term&,const Term&);

class InvertedIndexADT {
public:
	InvertedIndexADT(std::string filename);


	//ADT functions
	const Term& next(const std::string& term, int doc_num, int current);
	const Term& prev(const std::string& term, int doc_num, int current);


	// print the index DEBUG use
	void print_inveted_index();
private:
	//helper function
	void init_inveted_index(std::string filename);
	int binarySearch(const std::string& term,int low, int high, const Term& current, bool return_higher = true);

	//data members
	//inverted index
	//(term: (document occurrences, list(document#, index#)))
	 std::map<std::string,TermList> inverted_index{};

	 //last index positions for terms for next
	 //Galloping Search
	 std::map<std::string, std::size_t> c;
};

class FileNotExist{};

#endif /* INVERTEDINDEXADT_H_ */
