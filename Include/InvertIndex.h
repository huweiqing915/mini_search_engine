/*************************************************************************
	> File Name: InvertIndex.h
	> Author: huwq
	> Mail:huweiqing915@gmail.com 
	> Created Time: 2014年05月18日 星期日 22时09分34秒
 ************************************************************************/

#ifndef INVERT_INDEX_H_
#define INVERT_INDEX_H_

#include <unordered_map>
#include <map>
#include <string>
#include <set>
#include <utility>
#include <fstream>
#include <iomanip>  
#include <math.h>
#include <string.h>

#include "MixSegment.hpp"
#include "ExcludeSet.h"

class InvertIndex {
public:
	InvertIndex();
	~InvertIndex();
	void build_invert_index(const CppJieba::MixSegment &);
	void count_word_weight();
	void write_word_weight();
	void write_index();
private:
	//word : <docid : frequency>
	std::map<std::string, std::map<int, int> > _invert_index;
	//word : <docid : weight>
	std::map<std::string, std::map<int, float> > _word_weight;
	//存放docid - 词语 - weight 的map
	std::map<int, std::map<std::string, int> > _doc_word;
	int _docment_num;
};

#endif

