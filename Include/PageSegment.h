/*************************************************************************
	> File Name: PageSegment.h
	> Author: huwq
	> Mail:huweiqing915@gmail.com 
	> Created Time: 2014年05月16日 星期五 15时12分36秒
 ************************************************************************/
//把网页切词，统计词频，并产生topk

#ifndef PAGE_SEGMENT_H_
#define PAGE_SEGMENT_H_

#include <string>
#include <vector>
#include <queue>
#include <map>
#include <string.h>

#include "MixSegment.hpp"

#define TOPK 10

struct PageWord {
	std::string _word;
	int _frequency;
	PageWord(const std::string &word, int frequency):_word(word), _frequency(frequency)
	{
		//construct
	}
};

struct compare {	//提供给优先级队列priority_queue的比较方法
	bool operator()(const PageWord &a, const PageWord &b)
	{
		return a._frequency < b._frequency;
	}
};

class PageSegment {
public:
	PageSegment();
	~PageSegment();
	void build_word_queue(char *web_page, const CppJieba::MixSegment &);
	void set_del_status();
	std::vector<std::string>& get_word_vector();

	int doc_id;
	int length;
	std::string content;
	bool del_tag;

private:
	std::priority_queue<PageWord, std::vector<PageWord>, compare> _word_queue;
	std::set<std::string> _exclude;
	std::vector<std::string> _top_k;
	void put_topk_to_vector();
	// void init_path(std::string &dict_path, std::string &model_path);
};

#endif


