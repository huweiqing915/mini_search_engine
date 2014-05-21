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

void Search::search_result(const string &search_word, vector<pair<string, string> > &result_vec, const CppJieba::MixSegment &segment)
{
	EncodingConverter trans;
	string query_word;
	query_word = trans.utf8_to_gbk(search_word);
	//cut word
	vector<string> words;
	segment.cut(query_word, words);

	process_search_word(words);
	search_offset(trans, result_vec);
}

void Search::search_offset(EncodingConverter &trans, vector<pair<string, string> > &result_vec)
{
	Config *p = Config::get_instance();
	string pagelib_path;
	p->get_file_name("delduplicate_lib", pagelib_path);
	ifstream inlib;
	inlib.open(pagelib_path.c_str());

	for(multimap<float, int>::reverse_iterator riter = _sim_result_map.rbegin(); riter != _sim_result_map.rend(); ++riter)
	{
		//title - summary
		string result = _offset_vec[riter->second - 1];
		istringstream iss(result);
		int docid, offset, length;
		iss >> docid >> offset >> length;

		inlib.seekg(offset);
		char *buffer = new char[length + 1];
		inlib.read(buffer, length);
		buffer[length] = '\0';
		string doc = string(buffer);

		size_t pos_begin = doc.find("<title>");
		size_t pos_end = doc.find("</title>");
		size_t len = strlen("<title>");
		string title = doc.substr(pos_begin + len, pos_end - pos_begin - len - 1);

		pos_begin = doc.find("<content>");
		pos_end = doc.find("</content>");
		len = strlen("<content>");
		string content = doc.substr(pos_begin + len, pos_end - pos_begin - len - 1);

		title = trans.gbk_to_utf8(title);
		content = trans.gbk_to_utf8(content);

		result_vec.push_back(make_pair(title, content));
	}

	inlib.close();
}


void Search::process_search_word(vector<string> &words)
{
	ExcludeSet *p_exclude = ExcludeSet::get_instance();
	set<string> exclude = p_exclude->get_exclude_set();

	//存放每一个单词的docid的vector
	vector<vector<int> > idvec_vec;
	//存放查询词的map - word:tf
	map<string, int> query_map;
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
					//push_back(docid)
					id_tmp.push_back(x.first);
				}
				idvec_vec.push_back(id_tmp);
				id_tmp.clear();
			}	
		}
	}

#ifndef NDEBUG
	for(auto & ix : query_map)
	{
		cout << trans.gbk_to_utf8(ix.first) << " " << ix.second << endl;
	}
#endif

	//存放交集的结果
	vector<int> result;
	if(idvec_vec.size() > 1)
	{
		//求交集
		calculate_intersection(idvec_vec, result);
	}
	//如果只有一个词，则不需要求交集
	else if(idvec_vec.size() == 1)
	{
		result = idvec_vec[0];
	}
	else if(idvec_vec.size() == 0)
	{
		cout << "No result" << endl;
		return ;
	}

#ifndef NDEBUG
	cout << "------------" << endl;
	for(vector<int>::size_type ix = 0; ix != result.size(); ++ix)
	{
		cout << result[ix] << "\t";
	}
	cout << endl;
	cout << "idvec_vec.size: " << idvec_vec.size() << endl;
	cout << "result.size: " << result.size() << endl;
 	cout << "-------------" << endl;
#endif
 	//如果没有交集，则输出没找到
	if(result.size() == 0)
	{
		cout << "No result" << endl;
		return ;
	}
	//根据交集求相似度
	query_tf_idf(query_map);
	calculate_similar(result);
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

void Search::query_tf_idf(map<string, int> &query_map)
{
	//word:weight
	EncodingConverter trans;
	int N = _offset_vec.size();
	for(auto & x : query_map)
	{
		HashMapIter map_iter = _weight_map.find(x.first);
		int df = map_iter->second.size(); 
		float weight = x.second * log((N/df) + 0.05);
		_query_weight_map[x.first] = weight;
	}
	//对刚才的权重归一化，得到真正的权重
	float sum = 0.0;
	for(auto & ix : _query_weight_map)
	{
		sum += ix.second * ix.second;
	}
	for(auto & ix : _query_weight_map)
	{
		float tmp_weight = ix.second / sqrt(sum);
		cout << "---------------" << endl;
		cout << trans.gbk_to_utf8(ix.first) << endl;
		cout << "tmp_weight : " << tmp_weight << endl;
		cout << "----------------" << endl; 
		_query_weight_map[ix.first] = tmp_weight;
	}
}

//根据查询词的map，以及文档交集求相似度
void Search::calculate_similar(vector<int> &result)
{
	float sim;
	for(int ix = 0; ix != result.size(); ++ix)
	{
		sim = 0.0;
		//result[ix] = docid
		for(map<string, float>::iterator iter = _query_weight_map.begin(); 
						iter!= _query_weight_map.end(); ++iter)
		{
			sim += iter->second * (_weight_map[iter->first])[result[ix]];
		}
		_sim_result_map.insert(make_pair(sim, result[ix]));
		cout << "sim:" << sim << endl;
	}
	#ifndef NDEBUG
	for(multimap<float, int>::reverse_iterator riter = _sim_result_map.rbegin(); riter != _sim_result_map.rend(); ++riter)
	{
		cout << riter->first << "  " << riter->second << endl;
	}
	#endif
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

