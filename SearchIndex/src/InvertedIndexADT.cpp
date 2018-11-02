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

struct Accumulator {
	int docId;
	double score;
};

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
	doc_length.push_back(0);
	for (string line; getline(file,line);++indexNum) {
	        //New Doc if \n\n, reset indexNum
	    if (!line.size()) {
	    	// push to length
	    	doc_length.push_back(indexNum-1);
	    	//re-init
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
	doc_length.push_back(indexNum-1);
	doc_len_ave=0.0;
	for(auto t : doc_length) {
		doc_len_ave+=t;
		//cout << t << endl;
	}

	doc_len_ave/=doc_length.size();

	file.close();
}

std::multimap<double, int,greater<double>> InvertedIndexADT::rankCosine(std::vector<std::string>& terms) {
	std::multimap<double, int,greater<double>> p{};
	//Term current{0,0};
	int d=0;
	bool hasAllTerms = false;
	while(d<infinity.doc && !hasAllTerms) {
		for (auto t : terms) {
			Term n = next(t,d,0);
			d= max(d, n.doc);
		}
		hasAllTerms=true;
		for (auto t : terms)
			hasAllTerms*=inverted_index[t].document_occurence.count(d);
	}


	while (d < infinity.doc) {
		double score = 0.0;

		for (auto t : terms) {
			//find f(t,d)
			int f = -1;
			Term current {d,0};
			do {
				current = next(t,current.doc,current.index);
				++f;
			} while (current.doc == d);
			//cout << t << ' ' << (float)document_num/inverted_index[t].document_occurence.size() << ' ' << f << endl;
			score+= log2((double)document_num/inverted_index[t].document_occurence.size()) * (log2(f)+1);
		}

		p.insert(std::pair<double,int>{score,d});

		hasAllTerms=false;
		d++;
		while(d<infinity.doc && !hasAllTerms) {
				for (auto t : terms) {
					Term n = next(t,d,0);
					d= max(d, n.doc);
				}
				hasAllTerms=true;
				for (auto t : terms)
					hasAllTerms*=inverted_index[t].document_occurence.count(d);
			}

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

int InvertedIndexADT::nextDoc(std::string& term, int doc_num) {
	auto t = inverted_index[term].document_occurence.upper_bound(doc_num);
	if (t==inverted_index[term].document_occurence.end()) return INT_MAX;
	return t->first;
}

int InvertedIndexADT::prevDoc(std::string& term, int doc_num) {
	auto t = inverted_index[term].document_occurence.lower_bound(doc_num);
	if (t==inverted_index[term].document_occurence.begin()) return INT_MIN;
	return (--t)->first;
}

std::multimap<double, int, std::greater<double> > InvertedIndexADT::rankBM25_TermAtATimeWithPruning(std::vector<std::string>& terms,int k, int amax, int u) {
	std::multiset<std::pair<int,string>> st;
	for(string& t : terms)
		st.emplace(inverted_index.count(t) ? inverted_index[t].document_occurence.size() : 0, t);

	terms.clear();
	for(auto& t : st) {
		if (t.first != 0)
			terms.push_back(t.second);
		//cout << t.second << ' ' << t.first << endl;
	}

	vector<Accumulator> acc= vector<Accumulator>{}, accp =  vector<Accumulator>{};
	//acc.push_back(Accumulator{INT_MAX});
	for(int i = 0; i < (int)terms.size();++i) {
		int quotaLeft = amax-acc.size();
		int inPos, outPos;
		//cout << terms[i] << endl;
		if ((int)inverted_index[terms[i]].document_occurence.size() <= quotaLeft) {
			inPos=0; outPos=0;
			for (auto& d_p : inverted_index[terms[i]].document_occurence) {
				int d=d_p.first;
				while (inPos < (int)acc.size() && acc[inPos].docId < d) {
					accp.push_back(acc[inPos++]);
					++outPos;
				}

				//cout << log2(document_num/inverted_index[terms[i]].document_occurence[d])*TFBM25(terms[i], d) << endl;
				//cout << terms[i] << ':' << log2((double)document_num/inverted_index[terms[i]].document_occurence.size()) << endl;
				accp.push_back(Accumulator{d,log2((double)document_num/inverted_index[terms[i]].document_occurence.size())*TFBM25(terms[i], d)});

				if(inPos < (int)acc.size() && acc[inPos].docId == d) {
					accp[outPos].score += acc[inPos++].score;
				}

				outPos++;

			}
		} else if (quotaLeft == 0) { //no accumulators left
			for (int j = 0; j < (int)acc.size();++j)
				acc[j].score+=log2((double)document_num/inverted_index[terms[i]].document_occurence.size())*TFBM25(terms[i], acc[j].docId);


		} else { //still have some accumulators
			//TODO
			vector<int> tfStats(k,0);
			int T = 1;
			int postingsSeen=0;
			inPos=0;
			outPos=0;
			for (auto& d_p : inverted_index[terms[i]].document_occurence) {
				int d = d_p.first;
				while (inPos < (int) acc.size() && acc[inPos].docId < d) {
					accp.push_back(acc[inPos++]);
					++outPos;
				}
				if(inPos < (int) acc.size() && acc[inPos].docId == d) {
					accp.push_back(Accumulator{d, acc[inPos++].score + log2((double)document_num/inverted_index[terms[i]].document_occurence.size())*TFBM25(terms[i], d)});
					++outPos;
				} else if (quotaLeft > 0) {
                    if (inverted_index[terms[i]].document_occurence[d] >= T) { // if happens, make new accumlator
                    	accp.push_back(Accumulator{d, log2((double)document_num/inverted_index[terms[i]].document_occurence.size())*TFBM25(terms[i], d)});
                    	++outPos;
                        --quotaLeft;
                    }
                    if (inverted_index[terms[i]].document_occurence[d] < k) ++tfStats[inverted_index[terms[i]].document_occurence[d]];
                    else ++tfStats[k-1];
                }
				++postingsSeen;
				if (postingsSeen % u == 0) {
					double q = (double)(inverted_index[terms[i]].document_occurence.size()-postingsSeen)/postingsSeen;
					double sum=0.0;
					int x = 0;
					for(;sum < quotaLeft && x < k;++x) {
						sum+= q * tfStats[x];
					}
					T=x+1;
					//cout << quotaLeft << ' ' << q << ' ' << sum << endl;
				}

			}
		}

		while (inPos < (int) acc.size()) { // copy remaining acc to acc'
			accp.push_back(acc[inPos++]);
		}
		//accp.push_back(Accumulator{INT_MAX}); //end-of-list-marker
		//swap acc and acc'
		acc=std::move(accp);
		accp.clear();
	}

	std::multimap<double, int, std::greater<double>> r{};

	for (auto t : acc) {
		r.emplace(t.score,t.docId);
	}

	return r;
}

double InvertedIndexADT::TFBM25(std::string& term, int doc) {
	if (inverted_index[term].document_occurence.count(doc)) {
		auto ftd = inverted_index[term].document_occurence[doc];
		//cout << term << ':' << ftd*(1.2+1)/(ftd+1.2*(1-0.75+0.75*(doc_length[doc]/doc_len_ave))) << endl;

		return ftd*(1.2+1)/(ftd+1.2*(1-0.75+0.75*(doc_length[doc]/doc_len_ave)));
	} else return 0;
}
