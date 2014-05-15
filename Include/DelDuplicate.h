/*************************************************************************
	> File Name: DelDuplicate.h
	> Author: huwq
	> Mail:huweiqing915@gmail.com 
	> Created Time: 2014年05月15日 星期四 21时38分12秒
 ************************************************************************/
//网页去重，指纹法
#ifndef DEL_DUPLICATE_H_
#define DEL_DUPLICATE_H_

#include <string>
#include <map>
#include <vector>
#include <queue>

struct PageWord {
	int _docid;
	std::string _word;
	int _frequency;
};

class DelDuplicate {
public:
private:
	std::priority_queue<PageWord, std::vector<PageWord>, compare> _word_queue;
	std::set<std::string> _exclude;
};

#endif
