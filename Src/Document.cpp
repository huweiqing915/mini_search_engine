/*************************************************************************
	> File Name: Document.cpp
	> Author: huwq
	> Mail:huweiqing915@gmail.com 
	> Created Time: 2014年05月18日 星期日 16时08分56秒
 ************************************************************************/

#include "Document.h"
#include "Config.h"
#include "ExcludeSet.h"

using namespace std;

Document::Document():del_tag(false)
{

}

Document::~Document()
{

}

// inline void Document::init_path(string &dict_path, string &model_path)
// {
// 	Config *p = Config::get_instance();
// 	p->get_file_name("dict_path", dict_path);
// 	p->get_file_name("model_path", model_path);
// }

void Document::build_word_queue(char *web_page, const CppJieba::MixSegment &segment)
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

void Document::put_topk_to_vector()
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

std::vector<std::string>& Document::get_word_vector()
{
	return _top_k;
}

int Document::count_same_element(Document &other)
{
	vector<string> vec;
	vec = other.get_word_vector();

	int count = 0;
	vector<string>::iterator iter1 = _top_k.begin();
	while(iter1 != _top_k.end())
	{
		//find same word
		vector<string>::iterator iter = find_first_of(iter1, _top_k.end(), vec.begin(), vec.end());
		if(iter != _top_k.end())
		{
			++count;
			iter1 = ++iter;
		}
		else 
		{
			break;
		}
	}
#ifndef NDEBUG
	cout << "--------------" << endl;
	cout << "Reuturn count : " << count << endl;
	cout << "--------------" << endl;
#endif
	return count;
}

bool Document::operator==(Document &other)
{
	if(count_same_element(other) > SIM_MIN_NUM)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool Document::operator!= (Document &other)
{
	return !(*this == other);
}

void Document::set_del_status()
{
	del_tag = true;
}

