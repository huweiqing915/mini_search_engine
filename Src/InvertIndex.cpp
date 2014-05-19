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

void InvertIndex::build_invert_index(const CppJieba::MixSegment &segment)
{
	Config *p = Config::get_instance();
	string new_offset;
	string delduplicate_lib;
	p->get_file_name("new_offset", new_offset);
	p->get_file_name("delduplicate_lib", delduplicate_lib);

	ifstream infile;
	infile.open(new_offset.c_str());
	ifstream inlib;
	inlib.open(delduplicate_lib.c_str());

	int docid, offset, length;
	//取每一篇Page分别处理
	while(infile >> docid >> offset >> length)
	{
		cout << "-----------------" << endl;
		cout << docid <<"\t" << offset << "\t" << length << endl;
		// cout << "-----------------" << endl;
		
		_docment_num ++;
		inlib.seekg(offset);
		char *buffer = new char[length + 1];
		inlib.read(buffer, length);
		buffer[length] = '\0';
		string doc = string(buffer);

		vector<string> words;
		segment.cut(doc, words);

		ExcludeSet *p_exclude = ExcludeSet::get_instance();
		set<string> exclude = p_exclude->get_exclude_set();

		//build index
		for(vector<string>::iterator iter = words.begin(); iter != words.end(); ++iter)
		{
		//if not in the exclude
		//存放词语-词频的map
			if(!exclude.count(*iter))
			{
				++((_invert_index[*iter])[docid]); 
			}
		}
	}
}

void InvertIndex::count_word_weight()
{
	int df = 0;
	float weight = 0.0;
	for(auto & x : _invert_index)
	{
		//x.first -> word , x.second-> map<docid, frequency>;
		df = x.second.size();
		for(auto & y : x.second)
		{
			//y.first:docid, y.second:frequency;
			//w = tf*log(N/df)
			weight = y.second * log(_docment_num/df);
			(_word_weight[x.first])[y.first] = weight;
		}
	}
}

void InvertIndex::write_word_weight()
{
	Config *p = Config::get_instance();
	string word_weight;	
	p->get_file_name("word_weight", word_weight);
	ofstream outfile;
	outfile.open(word_weight.c_str());
	for(auto & x : _word_weight)
	{
		outfile << x.first << "\t";
		for(auto & y : x.second)
		{
			//归一化
			outfile << y.first << " " << setprecision(2) << y.second << "\t";
		}
		outfile << endl;
	}
	outfile.close();
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

