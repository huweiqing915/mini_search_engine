/*************************************************************************
	> File Name: ExcludeSet.h
	> Author: huwq
	> Mail:huweiqing915@gmail.com 
	> Created Time: 2014年05月15日 星期四 22时45分35秒
 ************************************************************************/
//停用词集，单例模式

#ifndef EXCLUDE_SET_H_
#define EXCLUDE_SET_H_

#include <fstream>
#include <string>
#include <set>

#include "MutexLock.h"

class ExcludeSet {
public:
	static ExcludeSet * get_instance();
	const std::set<std::string>& get_exclude_set();
	void build_exclude_set();
	~ExcludeSet();
private:
	ExcludeSet();
	static ExcludeSet *_p_exclude;
	static MutexLock _lock;
	std::set<std::string> _exclude_set; 
};

#endif
