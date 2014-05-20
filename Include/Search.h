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
#include <unordered_map>
#include <string>
#include <utility>

#include "EncodingConverter.h"
#include "MixSegment.hpp"
#include "ExcludeSet.h"

class Search {
public:
	typedef std::unordered_map<std::string, std::vector<std::pair<int, float> > > hash_map;
	typedef std::unordered_map<std::string, std::vector<std::pair<int, float> > >::iterator hash_map_iter;
	typedef std::unordered_map<int, std::unordered_map<std::string, float> >::iterator docid_map_iter;
	Search();
	~Search();
	void search_result(const string &, const CppJieba::MixSegment &);
private:
	std::string _search_word;
	std::unordered_map<std::string, std::vector<std::pair<int, float> > > _weight_map;
	//docid, <string:word, float:weight>
	std::unordered_map<int, std::unordered_map<std::string, float> > _docid_map;
	//用户查询词
	std::map<std::string, float> _query_word;
	void build_weight_map();
	float calculate_text_similar(const vector<pair<int, int> > &);
};

#endif