/*************************************************************************
	> File Name: Search.cpp
	> Author: huwq
	> Mail:huweiqing915@gmail.com 
	> Created Time: 2014年05月19日 星期一 16时28分53秒
 ************************************************************************/

#include "Search.h"
#include "Config.h"
#include "Log.h"

using namespace std;

Search::Search()
{
	cout << "Start overload files....." << endl;
	ExcludeSet *p_exclude = ExcludeSet::get_instance();
	_exclude = p_exclude->get_exclude_set();
	init_weight_map();
	init_offset_vec();
	init_pagelib_vec();
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
		map<int, float> tmp_map;
		while(iss >> docid >> weight)
		{
			tmp_map[docid] = weight;
		}
		_weight_map[keyword] = tmp_map;
		iss.clear();
	}
	infile.close();
	cout << "Overload Normalize_weight.index done ! " << endl;
}

//初始化<docid:偏移量>的那个索引
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
	cout << "Overload newoffset.index done ! " << endl;
}

//初始化网页库，将每个网页当成一个对象，放到vector中
void Search::init_pagelib_vec()
{
	Config *p = Config::get_instance();
	string pagelib;
	p->get_file_name("delduplicate_lib", pagelib);
	ifstream inlib;
	inlib.open(pagelib.c_str());

	EncodingConverter trans;
	for(auto & offset_ix : _offset_vec)
	{
		int docid, offset, length;
		istringstream iss(offset_ix);
		iss >> docid >> offset >> length;

		inlib.seekg(offset);
		char *buffer = new char[length + 1];
		inlib.read(buffer, length);
		buffer[length] = '\0';
		string doc = string(buffer);

		size_t pos_begin = doc.find("<title>");
		size_t pos_end = doc.find("</title>");
		size_t len = strlen("<title>");
		string title = doc.substr(pos_begin + len, pos_end - pos_begin - len);

		pos_begin = doc.find("<content>");
		pos_end = doc.find("</content>");
		len = strlen("<content>");
		string content = doc.substr(pos_begin + len, pos_end - pos_begin - len);

		string trans_title = trans.gbk_to_utf8(title);
		string trans_content = trans.gbk_to_utf8(content);

		_pagelib_vec.push_back(Document(docid, trans_title, trans_content));
	}
	cout << "Overload delduplicate_lib done !" << endl;

#ifndef NDEBUG
	cout << "input docid" << endl;
	int id;
	while(cin >> id)
	{
		cout << _pagelib_vec[id - 1]._title << endl;
		cout << _pagelib_vec[id - 1]._content << endl;
		cout << _pagelib_vec[id - 1]._docid << endl;
		cout << "----------------" << endl;
	}
#endif
}

void Search::search_result(const string &search_word, vector<Document> &result_vec, const CppJieba::MixSegment &segment)
{
	EncodingConverter trans;
	string query_word;
	query_word = trans.utf8_to_gbk(search_word);
	//cut word
	vector<string> words;
	words.clear();

	segment.cut(query_word, words);

	process_search_word(words);
	search_offset(result_vec);
#ifndef NDEBUG
	cout << "-------------" << endl;
	for(auto & ix : result_vec)
	{
		cout << ix._title << endl;
		cout << ix._content << endl;
	}
#endif
}

//根据docid在网页库中把文章找出来
void Search::search_offset(vector<Document> &result_vec)
{
	for(multimap<float, int>::reverse_iterator riter = _sim_result_map.rbegin(); riter != _sim_result_map.rend(); ++riter)
	{
		cout << "Simliar: " << riter->first << "  docid:" << riter->second << endl;
		result_vec.push_back(_pagelib_vec[riter->second - 1]);
	}
}

//切词 -> 求docid交集 -> 计算相似度
void Search::process_search_word(vector<string> &words)
{
	//存放每一个单词的docid的vector,用于求docid交集
	vector<vector<int> > idvec_vec;
	//存放查询词的map - word:tf
	map<string, int> query_map;
	//遍历用户查询词，切词后的vector
	EncodingConverter trans;
	for(vector<string>::iterator iter = words.begin(); iter != words.end(); ++iter)
	{
		//remove stop words;
		if(!_exclude.count(*iter))
		{
			cout << "----------cut_word---------" << endl;
			cout << trans.gbk_to_utf8(*iter) << endl;
			cout << "---------------------------" << endl;
			++query_map[*iter];
			//在索引中找这个词
			HashMapIter map_iter = _weight_map.find(*iter);
			//map_iter->second : map<docid, weight>
			//把每一个单词的docid放进vector中
			if(map_iter != _weight_map.end())
			{
				vector<int> id_tmp; 
				for(auto & x : map_iter->second)
				{
					//push_back(docid)
					id_tmp.push_back(x.first);
				}
				idvec_vec.push_back(id_tmp);
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
	vector<int> intersection;
	if(idvec_vec.size() > 1)
	{
		//求交集
		calculate_intersection(idvec_vec, intersection);
	}
	//如果只有一个词，则不需要求交集
	else if(idvec_vec.size() == 1)
	{
		intersection = idvec_vec[0];
	}
	//在索引中没有找到对应的单词，则返回没结果
	else if(idvec_vec.size() == 0)
	{
		cout << "No result....." << endl;
		return ;
	}

#ifndef NDEBUG
	cout << "------------" << endl;
	for(vector<int>::size_type ix = 0; ix != intersection.size(); ++ix)
	{
		cout << intersection[ix] << "\t";
	}
	cout << endl;
	cout << "idvec_vec.size: " << idvec_vec.size() << endl;
	cout << "intersection.size: " << intersection.size() << endl;
 	cout << "-------------" << endl;
#endif
 	//如果没有交集，则输出没找到
	if(intersection.size() == 0)
	{
		cout << "No intersection...." << endl;
		return ;
	}
	//根据交集求相似度
	query_tf_idf(query_map);
	calculate_similar(intersection);
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

//计算查询词的归一化权重
void Search::query_tf_idf(map<string, int> &query_map)
{
	//word:weight
	EncodingConverter trans;
	int N = _offset_vec.size();

	_query_weight_map.clear();

	for(auto & x : query_map)
	{
		HashMapIter map_iter = _weight_map.find(x.first);
		int df = map_iter->second.size(); 
		float weight = x.second * log((N/df) + 0.05);
		//_query_weight_map : <keyword, weight>
		_query_weight_map[x.first] = weight;
	}
	//对刚才的权重归一化，得到真正的权重
	float sum = 0.0;
	for(auto & ix : _query_weight_map)
	{
		sum += ix.second * ix.second;
	}
	for(auto & iy : _query_weight_map)
	{
		float tmp_weight = iy.second / sqrt(sum);
		cout << "---------------" << endl;
		cout << trans.gbk_to_utf8(iy.first) << endl;
		cout << "tmp_weight : " << tmp_weight << endl;
		cout << "----------------" << endl; 
		_query_weight_map[iy.first] = tmp_weight;
	}
}

//根据查询词的map，以及文档交集求相似度
void Search::calculate_similar(vector<int> &section)
{
	_sim_result_map.clear();
	for(int ix = 0; ix != section.size(); ++ix)
	{
		float sim = 0.0;
		//section[ix] = docid
		for(map<string, float>::iterator iter = _query_weight_map.begin(); iter!= _query_weight_map.end(); ++iter)
		{
			//iter->first:keyword;
			sim += iter->second * (_weight_map[iter->first])[section[ix]];
		}
		_sim_result_map.insert(make_pair(sim, section[ix]));
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

