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


struct TermDetail {
	std::map<int,int> document_occurence;
	//list(document#, index#)
	std::vector<std::vector<int>> term_index;
};

class InvertedIndexADT {
public:
	InvertedIndexADT(std::string filename);


	//ADT functions
	const std::vector<int>& next(std::string term, int doc_num, int current);
	const std::vector<int>& prev(std::string term, int doc_num, int current);


	// print the index DEBUG use
	void print_inveted_index();
private:
	//helper function
	void init_inveted_index(std::string filename);

	//data members
	//inverted index
	//(term: (document occurrences, list(document#, index#)))
	 std::map<std::string,TermDetail> inverted_index{};

	 //last index positions for terms for next
	 std::map<std::string, int> list_index_positions;
};

class FileNotExist{};

#endif /* INVERTEDINDEXADT_H_ */
