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

#define TOPK 15  //取出现次数排名前15的词
#define SIM_MIN_NUM 10	//相似度阈值

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

	void build_word_queue(char *web_page, const CppJieba::MixSegment &); //建立优先级队列
	void set_del_status();	//设置删除标记
	std::vector<std::string>& get_word_vector(); //取存放topk的vector

	bool operator==(Document &other);  //重载==运算符，用来比较两篇文档是否相似
	bool operator!=(Document &other);

	int _doc_id;	//文档id
	int _length;	//文档长
	std::string _content; //文档内容
	bool _del_tag;  //标记是否删除，true则表示已经删除

private:
	std::vector<std::string> _top_k;  //用来存放结果的vector
	int count_same_element(Document &other); //计算相同元素个数
	//把优先级队列中前几的词放入vector中
	void put_topk_to_vector(std::priority_queue<PageWord, std::vector<PageWord>, compare>&);	
};

#endif

