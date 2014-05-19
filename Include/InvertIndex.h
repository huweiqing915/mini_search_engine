/*************************************************************************
	> File Name: InvertIndex.h
	> Author: huwq
	> Mail:huweiqing915@gmail.com 
	> Created Time: 2014年05月18日 星期日 22时09分34秒
 ************************************************************************/

#ifndef INVERT_INDEX_H_
#define INVERT_INDEX_H_

#include <unordered_map>
#include <string>
#include <set>
#include <fstream>
#include <string.h>

#include "MixSegment.hpp"
#include "ExcludeSet.h"

class InvertIndex {
public:
	InvertIndex();
	~InvertIndex();
	void build_invert_index(const CppJieba::MixSegment &);
	void write_to_file();
private:
	//word : docid : frequency
	std::unordered_map<std::string, std::map<int, int> > _invert_index;
};

#endif

