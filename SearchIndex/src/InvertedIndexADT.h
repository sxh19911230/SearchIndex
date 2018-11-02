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
	std::pair<Term,Term> nextCover(const std::vector<std::string>&, int,int);

	int nextDoc(std::string& term, int doc_num);
	int prevDoc(std::string& term, int doc_num);

	//rankCosine return (score, doc_id)
	std::multimap<double,int,std::greater<double>> rankCosine(std::vector<std::string>&);

	//rankProximity
	std::multimap<double,int,std::greater<double>> rankProximity(std::vector<std::string>&);

	//rankBm25
	std::multimap<double,int,std::greater<double>> rankBM25_TermAtATimeWithPruning(std::vector<std::string>&,int=10, int=1000, int=128);

	//rankCover

	// print the index DEBUG use
	void print_inveted_index();
private:
	//helper function
	void init_inveted_index(std::string filename);
	int binarySearch(const std::string& term,int low, int high, const Term& current, bool return_higher = true);
	double TFBM25(std::string& term, int doc);

	//data members
	//inverted index
	//(term: (document occurrences, list(document#, index#)))
	 std::unordered_map<std::string,TermList> inverted_index{};
	 int document_num{};

	 //length of each doc
	 std::vector<int> doc_length;
	 double doc_len_ave{};

	 //last index positions for terms for next
	 //Galloping Search
	 std::unordered_map<std::string, std::size_t> c{};
};

class FileNotExist{};

#endif /* INVERTEDINDEXADT_H_ */
