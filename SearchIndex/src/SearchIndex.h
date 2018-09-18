/*
 * SearchIndex.h
 *
 *  Created on: 2018Äê9ÔÂ18ÈÕ
 *      Author: Forrest
 */

#ifndef SEARCHINDEX_H_
#define SEARCHINDEX_H_
#include <string>
#include <map>
#include <tuple>
#include <vector>
#include <memory>
#include <set>

class SearchIndex {
public:
	SearchIndex(std::string filename);
	void print_inveted_index();
private:
	//helper function
	void init_inveted_index(std::string filename);

	//data members
	//inverted index
	//(term: (document occurrences, list(document#, index#)))
	 std::map<std::string,std::tuple<std::set<int>,std::vector<std::unique_ptr<int[]>>>> inverted_index{};
	 int document_num=0;
};

class FileNotExist{};

#endif /* SEARCHINDEX_H_ */
