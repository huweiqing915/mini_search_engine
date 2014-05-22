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

Document::Document():_del_tag(false)
{

}

Document::~Document()
{

}

void Document::build_word_queue(char *web_page, const CppJieba::MixSegment &segment)
{
	vector<string> words;
	segment.cut(web_page, words);
	
	//Get exclude
	ExcludeSet *p_exclude = ExcludeSet::get_instance();
	set<string> exclude = p_exclude->get_exclude_set();

	//优先级队列，用来存储文档中的词
	priority_queue<PageWord, vector<PageWord>, compare> word_queue;

	map<string, int> word_map; //存放词语-词频的map
	for(vector<string>::iterator iter = words.begin(); iter != words.end(); ++iter)
	{
		//去停用词
		if(!exclude.count(*iter))
		{
			word_map[*iter] ++;
		}
	}

	//把word_map中的单词放进优先级队列中
	for(map<string, int>::iterator map_iter = word_map.begin(); map_iter != word_map.end(); ++map_iter)
	{
		word_queue.push(PageWord(map_iter->first, map_iter->second));
	}
	//把优先级队列中前15个数放入vector中
	put_topk_to_vector(word_queue);
}

void Document::put_topk_to_vector(priority_queue<PageWord, vector<PageWord>, compare> &word_queue)
{
	int num = 0;
	while(num != TOPK)
	{
		if(word_queue.empty())	//如果队列为空就结束
		{
			break;
		}
		string word = word_queue.top()._word;	
		word_queue.pop();
		if(word[0] & 0x80)	//is GBK
		{
			_top_k.push_back(word);
			num ++;	
		}
	}
}

vector<string>& Document::get_word_vector()
{
	return _top_k;
}

//计算相同元素个数
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
	_del_tag = true;
}

