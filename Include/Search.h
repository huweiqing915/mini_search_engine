/*************************************************************************
	> File Name: Search.h
	> Author: huwq
	> Mail:huweiqing915@gmail.com 
	> Created Time: 2014年05月19日 星期一 16时28分46秒
 ************************************************************************/
/*
 *	查询模块，输入查询词后，按照相似度从大到小输出结果
 */
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

struct Document {
	int _docid;
	std::string _title;
	std::string _content;
	Document() 
	{
		//default construct
	}
	Document(int docid, const std::string &title, const std::string &content):
			_docid(docid), _title(title), _content(content)
	{
		//construct
	}
};

class Search {
public:
	typedef std::unordered_map<std::string, std::map<int, float> > HashMap;
	typedef std::unordered_map<std::string, std::map<int, float> >::iterator HashMapIter;
	Search();
	virtual ~Search();

	void search_result(const std::string &, std::vector<Document> &, const CppJieba::MixSegment &);

	void debug();
private:
	//word - docid : weight
	std::unordered_map<std::string, std::map<int, float> > _weight_map;
	std::vector<std::string> _offset_vec;
	//every document vector
	std::vector<Document> _pagelib_vec;
	//stop word exlcude
	std::set<std::string> _exclude;
	//query word-weight
	std::map<std::string, float> _query_weight_map;
	//存放相似度排序结果的
	std::multimap<float, int> _sim_result_map;
	void init_weight_map();	//读入归一化化权重索引，放在内存中
	void init_offset_vec();	//读入偏移量索引，放在内存中
	void init_pagelib_vec(); //读入库文件，放在内存中

	void process_search_word(std::vector<std::string> &);	//处理查询词，计算权重，计算相似度
	void query_tf_idf(std::map<std::string, int> &);	//计算查询词的归一化权重
	void calculate_intersection(std::vector<std::vector<int> > &, std::vector<int> &); //求文档的交集
	void calculate_similar(std::vector<int> &r);	//计算相似度
	//根据docid，在网页库中查找文章，返回结果
	void search_offset(std::vector<Document> &);
};

#endif
