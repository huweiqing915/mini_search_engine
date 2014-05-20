/*************************************************************************
	> File Name: Search.cpp
	> Author: huwq
	> Mail:huweiqing915@gmail.com 
	> Created Time: 2014年05月19日 星期一 16时28分53秒
 ************************************************************************/

#include "Search"
#include "Config.h"

using namespace std;

void Search::build_weight_map()
{
	string index;
	Config *p = Config::get_instance();
	p->get_file_name("word_weight", index);
	ifstream infile;
	infile.open(index.c_str());
	string line;
	istringstream iss;
	//read line
	while(getline(infile, line))
	{
		iss.str(line);
		string keyword;
		int docid;
		int weight;
		iss >> keyword;
		vector<pair<int, float> > tmp;
		while(iss >> docid >> weight)
		{
			tmp.push_back(make_pair(docid, weight);
		}
		_weight_map[keyword] = tmp;
		iss.clear();
	}
	infile.close();
}

void Search::build_docid_map()
{
	string docid_index;
	Config *p = Config::get_instance();
	p->get_file_name("docid_index", docid_index);
	ifstream infile;
	infile.open(docid_index.c_str());
	string line;
	istringstream iss;
	//read line
	while(getline(infile, line))
	{
		iss.str(line);
		string keyword;
		int docid;
		int weight;
		iss >> docid;
		unoredered_map<string, float> > tmp;
		while(iss >> keyword >> weight)
		{
			tmp[keyword] = weight; 
		}
		_docid_map[docid] = tmp;
		iss.clear();
	}
	infile.close();
}

//根据docid,计算文本相似度
float Search::calculate_text_similar(const vector<pair<int, float> > &docid_weight_vec)
{
	vector<pair<int, int> >::const_iterator iter = docid_weight_vec.begin();
	vector<float> sim_vec; 
	//一个query与每一篇文档中的相似度
	while(iter != docid_weight_vec.end())
	{
		float sim = 0.0;
		//pair<int:docid, int:weight>
		//find docid, and get a unordered_map<word, weight> >
		//iter->first:docid
		//获得docid后找这篇文章的所有词
		docid_map_iter docid_iter = _docid_map.find(iter->first);
		//docid_iter.second:unordered_map
		unordered_map<string, float> every_docid_map = docid_iter->second;
		for(auto & x : _query_map)
		{
			if(erver_docid_map.find(x.first) != ervery_docid_map.end())
			{
				//一个query与一篇文章的相似度
				sim += every_docid_map.second * x.second; 
			}
		}
		sim_vec.push_back(sim);
		++iter;
	}
}

void Search::search_result(const string &search_word, const CppJieba::MixSegment &segment)
{
	EncodingConverter trans;
	string s;
	s = trans.utf8_to_gbk(search_word);
	vector<string> words;
	segment.cut(s, words);

	ExcludeSet *p_exclude = ExcludeSet::get_instance();
	set<string> exclude = p_exclude->get_exclude_set();

	//find user's word
	for(vector<string>::iterator iter = words.begin(); iter != words.end; ++iter)
	{
		//remove stop words;
		if(!exclude.count(*iter))
		{
		//*iter
			hash_map_iter map_iter = _weight_map.find(*iter);
			if(map_iter != _weight_map.end())
			{
				//find in vector
				calculate_text_similar(map_iter->second, );	
			}
		}
	}
}

void debug()
{
	for(auto & x : _weight_map)
	{
		cout << x.first << "\t";
		//x.first : string x.second : vector<pair<int, int> >
		for(auto & y : x.second)
		{
			cout << y.first << " " << y.second << endl;
		}
	}
}

