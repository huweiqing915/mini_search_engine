/*************************************************************************
	> File Name: DelDuplicate.h
	> Author: huwq
	> Mail:huweiqing915@gmail.com 
	> Created Time: 2014年05月15日 星期四 21时38分12秒
 ************************************************************************/
//网页去重，特征码
//每个网页都有一个优先级队列
#ifndef DEL_DUPLICATE_H_
#define DEL_DUPLICATE_H_

#include <string>
#include "EncodingConverter.h"

class DelDuplicate {
public:
	DelDuplicate();
	~DelDuplicate();

	void build_feature_code();

	int _docid;
	int _length;
	std::string _content;
	bool _del_tag;

private:
	std::string _feature_code;
};

#endif

