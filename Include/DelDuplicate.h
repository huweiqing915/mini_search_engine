/*************************************************************************
	> File Name: DelDuplicate.h
	> Author: huwq
	> Mail:huweiqing915@gmail.com 
	> Created Time: 2014年05月15日 星期四 21时38分12秒
 ************************************************************************/
//网页去重，特征码

#ifndef DEL_DUPLICATE_H_
#define DEL_DUPLICATE_H_

#include <string>
#include <vector>

#include "FeatureCode.h"
#include "Config.h"

#define SIM_MIN_NUM 0.3

class DelDuplicate {
public:
	DelDuplicate();
	~DelDuplicate();
	void build_page_vector();
	void delete_duplicate_page();
	void write_to_file();
private:
	std::vector<FeatureCode> _del_vector;
	void parse_gbk_string(const std::string &str, std::vector<uint16_t> &vec); 
	int longest_common_sequence(const std::string &str1, const std::string &str2);
};

#endif

