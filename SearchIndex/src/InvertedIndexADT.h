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
#include <unordered_map>
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
	const Term& next(const std::string& term, int doc_num, int ind_num);
	const Term& prev(const std::string& term, int doc_num, int ind_num);

	//These two probably will not be implemented
	int nextDoc(std::string& term, int doc_num);
	int prevDoc(std::string& term, int doc_num);

	//rankCosine
	std::multimap<double,int,std::greater<double>> rankCosine(std::vector<std::string>&);

	// print the index DEBUG use
	void print_inveted_index();
private:
	//helper function
	void init_inveted_index(std::string filename);
	int binarySearch(const std::string& term,int low, int high, const Term& current, bool return_higher = true);

	//data members
	//inverted index
	//(term: (document occurrences, list(document#, index#)))
	 std::unordered_map<std::string,TermList> inverted_index{};
	 int document_num{};

	 //last index positions for terms for next
	 //Galloping Search
	 std::unordered_map<std::string, std::size_t> c{};
};

class FileNotExist{};

#endif /* INVERTEDINDEXADT_H_ */
