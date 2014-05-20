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
		
		_docment_num ++;	//count N

		inlib.seekg(offset);
		char *buffer = new char[length + 1];
		inlib.read(buffer, length);
		buffer[length] = '\0';
		string doc = string(buffer);

		size_t pos_begin = doc.find("<content>");
		size_t pos_end = doc.find("</content>");
		size_t len = strlen("<content>");
		doc = doc.substr(pos_begin + len, pos_end - pos_begin - len - 1);

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
		df = x.second.size();	//df
		for(auto & y : x.second)
		{
			//y.first:docid, y.second:frequency;
			//w = tf*log(N/df)
			weight = y.second * log((_docment_num/df) + 0.05);
			// (_word_weight[x.first])[y.first] = weight;
			(_doc_word[y.first])[x.first] = weight;
		}
	}
	_invert_index.clear();
}

void InvertIndex::write_word_weight()
{
	Config *p = Config::get_instance();
	string word_weight;	
	p->get_file_name("weight_index", word_weight);
	ofstream outfile;
	outfile.open(word_weight.c_str());
	float tmp = 0.0;
	float sum;
	for(auto & x : _doc_word)
	{
		sum = 0.0;
		//x.first : docid, x.second:<word, weight>
		//每一篇文章，归一化
		cout << "-------doc size--------" << endl;
		cout << x.second.size() << endl;
		cout << "------------------------" << endl;
		for(auto & ix : x.second)
		{
			//wi的平方和
			sum += ix.second * ix.second;
		}
		for(auto & y : x.second)
		{
			//y.second:weight
			tmp =  y.second / sqrt(sum);
			(_word_weight[y.first])[x.first] = tmp;
		}
	}

	//write to file
	for(auto & x : _word_weight)
	{
		outfile << x.first << "\t";
		for(auto & y : x.second)
		{
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

