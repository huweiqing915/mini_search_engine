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

	void search_result(const std::string &, std::vector<std::pair<std::string, std::string> > &, const CppJieba::MixSegment &);

	void debug();
private:
	//word - docid : weight
	std::unordered_map<std::string, std::map<int, float> > _weight_map;
	std::vector<std::string> _offset_vec;
	//query word-weight
	std::map<std::string, float> _query_weight_map;
	//存放相似度排序结果的
	std::multimap<float, int> _sim_result_map;
	void init_weight_map();
	void init_offset_vec();
	void process_search_word(std::vector<std::string> &);
	void query_tf_idf(std::map<std::string, int> &);
	void calculate_intersection(std::vector<std::vector<int> > &, std::vector<int> &); //求文档的交集
	void calculate_similar(std::vector<int> &r);
	void search_offset(EncodingConverter &, std::vector<std::pair<std::string, std::string> > &);
};

#endif
