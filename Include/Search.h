/*************************************************************************
	> File Name: Search.h
	> Author: huwq
	> Mail:huweiqing915@gmail.com 
	> Created Time: 2014年05月19日 星期一 16时28分46秒
 ************************************************************************/

#ifndef SEARCH_H_
#define SEARCH_H_

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

#include "EncodingConverter.h"
#include "MixSegment.hpp"
#include "ExcludeSet.h"

class Search {
public:
	typedef std::unordered_map<std::string, std::map<int, float> > HashMap;
	typedef std::unordered_map<std::string, std::map<int, float> >::iterator HashMapIter;
	Search();
	virtual ~Search();

	void process_search_word(const std::string &, const CppJieba::MixSegment &);
	void debug();
private:
	//word - docid : weight
	std::unordered_map<std::string, std::map<int, float> > _weight_map;
	std::vector<std::string> _offset_vec;
	//query word-weight
	map<string, float> _word_weight_map;
	void init_weight_map();
	void init_offset_vec();
	float calculate_text_similar(const std::vector<std::pair<int, int> > &);
	void calculate_intersection(std::vector<std::vector<int> > &, std::vector<int> &); //求文档的交集
};

#endif
