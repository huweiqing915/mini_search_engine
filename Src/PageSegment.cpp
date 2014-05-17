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

PageSegment::PageSegment():del_tag(false)
{

}

PageSegment::~PageSegment()
{

}

// inline void PageSegment::init_path(string &dict_path, string &model_path)
// {
// 	Config *p = Config::get_instance();
// 	p->get_file_name("dict_path", dict_path);
// 	p->get_file_name("model_path", model_path);
// }

void PageSegment::build_word_queue(char *web_page, const CppJieba::MixSegment &segment)
{
	vector<string> words;
	segment.cut(web_page, words);
	
	//Get exclude
	ExcludeSet *p_exclude = ExcludeSet::get_instance();
	set<string> exclude = p_exclude->get_exclude_set();

	//build word_map
	map<string, int> word_map; //存放词语-词频的map
	for(vector<string>::iterator iter = words.begin(); iter != words.end(); ++iter)
	{
		//if not in the exclude
		if(!exclude.count(*iter))
		{
			word_map[*iter] ++;
		}
	}

	//words are in the map, then put it into priority_queue
	for(map<string, int>::iterator map_iter = word_map.begin(); 
		map_iter != word_map.end(); ++map_iter)
	{
		_word_queue.push(PageWord(map_iter->first, map_iter->second));
	}
	//put queue to vector
	put_topk_to_vector();
}

void PageSegment::put_topk_to_vector()
{
	int num = 0;
	while(num != TOPK)
	{
		if(_word_queue.empty())	//if is empty
		{
			break;
		}
		string word = _word_queue.top()._word;
		_word_queue.pop();
		if(word[0] & 0x80)	//is GBK
		{
			_top_k.push_back(word);
			num ++;	
		}
	}
}

std::vector<std::string>& PageSegment::get_word_vector()
{
	return _top_k;
}

void PageSegment::set_del_status()
{
	del_tag = true;
}