/*************************************************************************
	> File Name: Document.h
	> Author: huwq
	> Mail:huweiqing915@gmail.com 
	> Created Time: 2014年05月18日 星期日 16时09分11秒
 ************************************************************************/

#ifndef DOCUMENT_H_
#define DOCUMENT_H_

#include <string>
#include <vector>
#include <queue>
#include <map>
#include <string.h>

#include "MixSegment.hpp"

#define TOPK 10
#define SIM_MIN_NUM 8

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

class Document {
public:
	Document();
	~Document();
	void build_word_queue(char *web_page, const CppJieba::MixSegment &);
	void set_del_status();
	std::vector<std::string>& get_word_vector();

	bool operator==(Document &other);
	bool operator!= (Document &other);

	int doc_id;
	int length;
	std::string content;
	bool del_tag;

private:
	std::priority_queue<PageWord, std::vector<PageWord>, compare> _word_queue;
	std::set<std::string> _exclude;
	std::vector<std::string> _top_k;
	int count_same_element(Document &other);
	void put_topk_to_vector();
};

#endif

