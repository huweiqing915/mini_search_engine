/*************************************************************************
	> File Name: InvertIndex.cpp
	> Author: huwq
	> Mail:huweiqing915@gmail.com 
	> Created Time: 2014年05月18日 星期日 21时12分08秒
 ************************************************************************/

#include "InvertIndex.h"
#include "Config.h"

using namespace std;

InvertIndex::InvertIndex():_docment_num(0)
{

}

InvertIndex::~InvertIndex()
{

}

void InvertIndex::build_invert_index()
{
	Config *p = Config::get_instance();
	string new_offset;
	string delduplicate_lib;
	string dict_path;
	string model_path;
	//读取需要的文件
	p->get_file_name("dict_path", dict_path);
	p->get_file_name("model_path", model_path);
	p->get_file_name("new_offset", new_offset);
	p->get_file_name("delduplicate_lib", delduplicate_lib);

	//初始化切词工具
	CppJieba::MixSegment segment(dict_path, model_path);

	ifstream infile;
	infile.open(new_offset.c_str());
	ifstream inlib;
	inlib.open(delduplicate_lib.c_str());

	ExcludeSet *p_exclude = ExcludeSet::get_instance();
	set<string> exclude = p_exclude->get_exclude_set();

	int docid, offset, length;
	//取每一篇Page分别处理
	while(infile >> docid >> offset >> length)
	{
		cout << "-----------------" << endl;
		cout << docid <<"\t" << offset << "\t" << length << endl;
		// cout << "-----------------" << endl;
		
		_docment_num ++;	//count N

		inlib.seekg(offset);
		char *buffer = new char[length + 1];
		inlib.read(buffer, length);
		buffer[length] = '\0';
		string doc = string(buffer);

		size_t pos_begin = doc.find("<content>");
		size_t pos_end = doc.find("</content>");
		size_t len = strlen("<content>");
		//取出标签content中的内容
		doc = doc.substr(pos_begin + len, (pos_end - pos_begin) - len);

		vector<string> words;
		segment.cut(doc, words);

		//build index
		for(vector<string>::iterator iter = words.begin(); iter != words.end(); ++iter)
		{
			//去停用词, invert_inde:存放词语:(docid,词频)的map
			if(!exclude.count(*iter))
			{
				++((_invert_index[*iter])[docid]);
			}
		}
	}
}

//根据倒排索引表计算各个单词的权重
void InvertIndex::count_word_weight()
{
	//_invert_index : map<keyword, map<docid, tf> >
	for(auto & x : _invert_index)
	{
		//x.first -> word , x.second-> map<docid, tf>;
		int df = x.second.size();	//df
		for(auto & y : x.second)
		{
			//y.first:docid, y.second:tf;
			//w = tf*log(N/df + 0.05)
			float weight = y.second * log((_docment_num/df) + 0.05);
			//_doc_word : map<docid, map<keyword, float> >
			(_doc_word[y.first])[x.first] = weight;
		}
	}
	_invert_index.clear();
}

void InvertIndex::write_index()
{
	Config *p = Config::get_instance();
	string index;	
	p->get_file_name("invert_index", index);
	ofstream outfile;
	outfile.open(index.c_str());

	for(auto & x : _invert_index)
	{
		outfile << x.first << "\t";
		for(auto & y : x.second)
		{
			outfile << y.first << " " << y.second << "\t";
		}
		outfile << endl;
	}
	outfile.close();
}

void InvertIndex::write_word_weight()
{
	Config *p = Config::get_instance();
	string word_weight;	
	p->get_file_name("weight_index", word_weight);
	ofstream outfile;
	outfile.open(word_weight.c_str());

	for(auto & x : _doc_word)
	{
		float sum = 0.0;
		//x.first : docid, x.second:<word, weight>
		//每一篇文章，归一化
		cout << "-------doc size--------" << endl;
		cout << x.second.size() << endl;
		for(auto & ix : x.second)
		{
			//wi的平方和,ix: map<word,weight>
			sum += ix.second * ix.second;
		}
		for(auto & iy : x.second)
		{
			//iy.second:weight,归一化结果放入新的map中去
			float normalize_weight = iy.second / sqrt(sum);
			(_word_weight[iy.first])[x.first] = normalize_weight;
		}
	}

	//write to file
	for(auto & x : _word_weight)
	{
		//x.first : word, x.second map<docid, normalize_weight>
		outfile << x.first << "\t";
		for(auto & y : x.second)
		{
			outfile << y.first << " " << setprecision(2) << y.second << "\t";
		}
		outfile << endl;
	}

	outfile.close();
}

