/*************************************************************************
	> File Name: Search.cpp
	> Author: huwq
	> Mail:huweiqing915@gmail.com 
	> Created Time: 2014年05月19日 星期一 16时28分53秒
 ************************************************************************/

#include "Search.h"
#include "Config.h"

using namespace std;

Search::Search()
{
	init_weight_map();
	init_offset_vec();
}

Search::~Search()
{

}

void Search::init_weight_map()
{
	string index;
	Config *p = Config::get_instance();
	p->get_file_name("weight_index", index);
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
		float weight;
		iss >> keyword;
		map<int, float> tmp;
		while(iss >> docid >> weight)
		{
			tmp[docid] = weight;
		}
		_weight_map[keyword] = tmp;
		iss.clear();
	}
	infile.close();
}

//初始化docid，偏移量的那个索引
void Search::init_offset_vec()
{
	//open file;
	Config *p = Config::get_instance();
	string offset;
	p->get_file_name("new_offset", offset);
	ifstream infile;
	infile.open(offset.c_str());

	string line;
	while(getline(infile, line))
	{
		_offset_vec.push_back(line);
	}

	infile.close();
}

//根据docid,计算文本相似度
// float Search::calculate_text_similar(const vector<pair<int, float> > &docid_weight_vec)
// {
// 	vector<pair<int, int> >::const_iterator iter = docid_weight_vec.begin();
// 	vector<float> sim_vec; 
// 	//一个query与每一篇文档中的相似度
// 	while(iter != docid_weight_vec.end())
// 	{
// 		float sim = 0.0;
// 		//pair<int:docid, int:weight>
// 		//find docid, and get a unordered_map<word, weight> >
// 		//iter->first:docid
// 		//获得docid后找这篇文章的所有词
// 		docid_map_iter docid_iter = _docid_map.find(iter->first);
// 		//docid_iter.second:unordered_map
// 		unordered_map<string, float> every_docid_map = docid_iter->second;
// 		for(auto & x : _query_map)
// 		{
// 			if(erver_docid_map.find(x.first) != ervery_docid_map.end())
// 			{
// 				//一个query与一篇文章的相似度
// 				sim += every_docid_map.second * x.second; 
// 			}
// 		}
// 		sim_vec.push_back(sim);
// 		++iter;
// 	}
// }

void Search::calculate_intersection(vector<vector<int> > &idvec_vec, vector<int> &result)
{
	vector<int>::iterator iter;

	for(int ix = 0; ix != idvec_vec.size(); ++ix)
	{
		if(ix == 0)
		{
			set_intersection(idvec_vec[ix].begin(), idvec_vec[ix].end(), idvec_vec[ix + 1].begin(),
				idvec_vec[ix + 1].end(), back_inserter(result));
		}
		else
		{
			iter = set_intersection(result.begin(), result.end(), idvec_vec[ix].begin(), idvec_vec[ix].end(), result.begin());
			result.resize(iter - result.begin());
		}
	}
}

void Search::search_result(const string &search_word, const CppJieba::MixSegment &segment)
{
	EncodingConverter trans;
	string query_word;
	query_word = trans.utf8_to_gbk(search_word);
	//cut word
	vector<string> words;
	segment.cut(query_word, words);

	for(auto & x : words)
	{
		cout << "----------" << endl;
		cout << x << endl;
		cout << "-------------" << endl;
	}

	ExcludeSet *p_exclude = ExcludeSet::get_instance();
	set<string> exclude = p_exclude->get_exclude_set();

	//存放每一个单词的docid的vector
	vector<vector<int> > idvec_vec; 
	//find user's word
	cout << "------------begin--------" << endl;
	for(vector<string>::iterator iter = words.begin(); iter != words.end(); ++iter)
	{
		vector<int> id_tmp;
		//remove stop words;
		if(!exclude.count(*iter))
		{
			//*iter
			HashMapIter map_iter = _weight_map.find(*iter);
			//map_iter->second : vector<pair<int, float> >
			//把每一个单词的docid放进vector中
			for(auto & x : map_iter->second)
			{
				id_tmp.push_back(x.first);
			}	
		}
		idvec_vec.push_back(id_tmp);
		id_tmp.clear();
	}
	cout << "-------------" << endl;
	cout << "idvec_vec.size()" << idvec_vec.size() << endl;
	cout << "-------------" << endl;
	vector<int> result;
	if(idvec_vec.size() > 1)
	{
		//求交集
		calculate_intersection(idvec_vec, result);
	}
	//如果只有一个词，则不需要求交集
	else
	{
		result = idvec_vec[0];
	}

	for(vector<int>::size_type ix = 0; ix != result.size(); ++ix)
	{
		cout << result[ix] << "\t";
	}
	cout << endl;
	//根据交集求相似度

}

// void Search::debug()
// {
// 	for(auto & x : _weight_map)
// 	{
// 		cout << x.first << "\t";
// 		//x.first : string x.second : vector<pair<int, int> >
// 		for(auto & y : x.second)
// 		{
// 			cout << y.first << " " << y.second << "\t";
// 		}
// 		cout << endl;
// 	}
// 	cout << "-------------" << endl;
// 	cout << "lines:" << _offset_vec.size() << endl;
// 	cout << "-------------" << endl;
// }

