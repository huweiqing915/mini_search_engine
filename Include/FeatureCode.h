/*************************************************************************
	> File Name: FeatureCode.h
	> Author: huwq
	> Mail:huweiqing915@gmail.com 
	> Created Time: 2014年05月18日 星期日 11时15分58秒
 ************************************************************************/
//获取网页的特征码

#ifndef FEATURE_CODE_H_
#define FEATURE_CODE_H_

#include <string>
#include "EncodingConverter.h"

#define CHINESE_PUNCT  "，"	//提取特征码的中文标点符号
#define ENGLISH_PUNCT  ','	//提取特征码的英文标点符号
#define CUT_LEN 14	//截取标点符号前后的长度

class FeatureCode {
public:
	FeatureCode();
	~FeatureCode();

	void build_feature_code();
	const std::string& get_feature_code();
	int feature_code_size() const;
	void set_del_status();
	void debug();

	int _docid;
	int _length;
	bool _del_tag;
	std::string _content;

private:
	std::string _feature_code;
};

#endif
