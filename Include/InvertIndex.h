/*************************************************************************
	> File Name: InvertIndex.h
	> Author: huwq
	> Mail:huweiqing915@gmail.com 
	> Created Time: 2014年05月18日 星期日 22时09分34秒
 ************************************************************************/
/*
 *  根据去重后的网页库建立倒排索引,以及归一化后的（词：（文档id，权重））索引
 */
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

	void build_invert_index();	//建立倒排索引表
	void count_word_weight();	//计算词的权重
	void write_word_weight();	//写入文件中	
	void write_index();
private:
	//word : <docid : frequency>
	std::map<std::string, std::map<int, int> > _invert_index;
	//word : <docid : weight>
	std::map<std::string, std::map<int, float> > _word_weight;
	//存放docid - 词语 - weight 的map
	std::map<int, std::map<std::string, float> > _doc_word;
	int _docment_num;
};

#endif

