/*************************************************************************
	> File Name: ExcludeSet.cpp
	> Author: huwq
	> Mail:huweiqing915@gmail.com 
	> Created Time: 2014年05月15日 星期四 22时45分46秒
 ************************************************************************/

#include "ExcludeSet.h"
#include "Config.h"

using namespace std;

ExcludeSet * ExcludeSet::_p_exclude = NULL;
MutexLock ExcludeSet::_lock;

ExcludeSet::ExcludeSet()
{
	build_exclude_set();
}

ExcludeSet::~ExcludeSet()
{

}

ExcludeSet* ExcludeSet::get_instance()
{
	if(_p_exclude == NULL)
	{
		_lock.lock();
		if(_p_exclude == NULL)
		{
			_p_exclude = new ExcludeSet;
		}
		_lock.unlock();
	}
	return _p_exclude;
}

void ExcludeSet::build_exclude_set()
{
	Config *p = Config::get_instance();
	string stop_list;
	p->get_file_name("stop_list", stop_list);
	ifstream infile;
	infile.open(stop_list.c_str());
	string remove_word;
	while(infile >> remove_word)
	{
		_exclude_set.insert(remove_word);
	}
	infile.close();
	infile.clear();
}

const set<string>& ExcludeSet::get_exclude_set()
{
	return _exclude_set;
}
