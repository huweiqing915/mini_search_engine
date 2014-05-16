/*************************************************************************
	> File Name: PageSegment.cpp
	> Author: huwq
	> Mail:huweiqing915@gmail.com 
	> Created Time: 2014年05月16日 星期五 15时14分54秒
 ************************************************************************/

#include "PageSegment.h"
#include "Config.h"
#include "ExcludeSet.h"

using namespace std;

PageSegment::PageSegment()
{

}

PageSegment::~PageSegment()
{

}

void PageSegment::init_path(string &dict_path, string &model_path)
{
	Config *p = Config::get_instance();
	p->get_file_name("dict_path", dict_path);
	p->get_file_name("model_path", model_path);
}

void PageSegment::build_word_queue(char *web_page)
{
	string dict_path;
	string model_path;
	init_path(dict_path, model_path);
	
	CppJieba::MixSegment segment(dict_path, model_path);
	vector<string> words;
	segment.cut(web_page, words);

	//Get exclude
	ExcludeSet *p_exclude = ExcludeSet::get_instance();
	set<string> exclude = p_exclude->get_exclude_set();
// #ifndef NDEBUG
// 	for(auto & x : exclude)
// 	{
// 		cout << x << endl;
// 	}
// 	cout << "---------------------" << endl;
// #endif
	//build word_map
	map<string, int> word_map; //存放词语-词频的map
	for(vector<string>::iterator iter = words.begin(); iter != words.end(); ++iter)
	{
		//if not in the exclude
		if(!exclude.count(*iter))
		{
			// #ifndef NDEBUG
			// 	cout << "-------------" << endl;
			// 	cout << *iter << endl;
			// #endif
			word_map[*iter] ++;
		}
	}

	//words are in the map, then put it into priority_queue
	for(map<string, int>::iterator map_iter = word_map.begin(); 
		map_iter != word_map.end(); ++map_iter)
	{
		_word_queue.push(PageWord(map_iter->first, map_iter->second));
	}
}

string PageSegment::get_top_word()
{
	string ret = _word_queue.top()._word;
	_word_queue.pop();
	return ret;
}
