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

//求文档的交集
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

void Search::process_search_word(const string &search_word, const CppJieba::MixSegment &segment)
{
	EncodingConverter trans;
	string query_word;
	query_word = trans.utf8_to_gbk(search_word);
	//cut word
	vector<string> words;
	segment.cut(query_word, words);

	ExcludeSet *p_exclude = ExcludeSet::get_instance();
	set<string> exclude = p_exclude->get_exclude_set();

	//存放每一个单词的docid的vector
	vector<vector<int> > idvec_vec;
	//存放查询词的map - word:tf
	map<string, int> query_map;
	//docid - word - weight
	map<int, map<string, float> > id_map_map;
	//遍历用户查询词，切词后的vector
	for(vector<string>::iterator iter = words.begin(); iter != words.end(); ++iter)
	{
		vector<int> id_tmp; 
		//remove stop words;
		if(!exclude.count(*iter))
		{
			#ifndef NDEBUG
			cout << trans.gbk_to_utf8(*iter) << endl;
			#endif
			++query_map[*iter];
			//*iter
			HashMapIter map_iter = _weight_map.find(*iter);
			//map_iter->second : map<int, float>
			//把每一个单词的docid放进vector中
			if(map_iter != _weight_map.end())
			{
				for(auto & x : map_iter->second)
				{
					//x.first:docid, *iter:word, x.second:weight
					(id_map_map[x.first])[*iter] = x.second;
					//push_back(docid)
					id_tmp.push_back(x.first);
				}
				idvec_vec.push_back(id_tmp);
				id_tmp.clear();
			}	
		}
	}

	//存放交集的结果
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
#ifndef NDEBUG
	for(vector<int>::size_type ix = 0; ix != result.size(); ++ix)
	{
		cout << result[ix] << "\t";
	}
	cout << endl;
#endif
	//根据交集求相似度
	calculate_similar(result);
}

void Search::query_tf_idf(map<string, int> &query_map)
{
	//word:weight
	int N = _offset_vec.size();
	for(auto & x : query_map)
	{
		HashMapIter map_iter = _weight_map.find(*iter);
		int df = map_iter->second.size(); 
		float weight = x.second * log((N/df) + 0.05);
		_word_weight_map[x.first] = weight;
	}
	//对刚才的权重归一化，得到真正的权重
	float sum = 0.0
	for(auto & ix : _word_weight_map)
	{
		sum += ix.second * ix.second;
	}
	for(auto & ix : _word_weight_map)
	{
		float tmp_weight = ix.second / sqrt(sum);
		_word_weight_map[ix.first] = tmp_weight;
	}
}

//根据查询词的map，以及文档交集求相似度
void Search::calculate_similar(vector<int> &result, map<int, map<string, float> > &id_map_map)
{
	map<float, int> sim_result_map;
	float sim = 0.0
	for(int ix = 0; ix != result.size(); ++ix)
	{
		sim = 0.0;
		//result[ix] = docid
		map<string, float> tmp_map = id_map_map[result[ix]];
		map<string, float>::iterator iter1 = tmp_map.begin();
		map<string, float>::iterator iter2 = _word_weight_map.begin();
		while(iter1 != tmp_map.end() && iter2 != _word_weight_map.end())
		{
			sim += iter1->second * iter2->second;
			iter1 ++;
			iter2 ++;
		}
		sim_result_map[sim] = result[ix];
	}
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

