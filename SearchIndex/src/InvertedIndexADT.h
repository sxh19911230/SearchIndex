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

class InvertedIndexADT {
public:
	InvertedIndexADT(std::string filename);


	//ADT functions
	const Term& next(std::string term, int doc_num, int current);
	const Term& prev(std::string term, int doc_num, int current);


	// print the index DEBUG use
	void print_inveted_index();
private:
	//helper function
	void init_inveted_index(std::string filename);

	//data members
	//inverted index
	//(term: (document occurrences, list(document#, index#)))
	 std::map<std::string,TermList> inverted_index{};

	 //last index positions for terms for next
	 std::map<std::string, int> list_index_positions;
};

class FileNotExist{};

#endif /* INVERTEDINDEXADT_H_ */
