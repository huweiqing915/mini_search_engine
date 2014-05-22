/*************************************************************************
> File Name: test_delpage.cpp
> Author: huwq
> Mail:huweiqing915@gmail.com 
> Created Time: 2014年05月21日 星期三 22时19分21秒
************************************************************************/

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <string.h>
#include <ostream>
#include "Config.h"
#include "PageSegment.h"

#define SIM_MIN_NUM 8

using namespace std;

//count the same element number
int count_same_element(PageSegment &ps_a, PageSegment &ps_b)
{
	vector<string> vec1;
	vector<string> vec2;
	vec1 = ps_a.get_word_vector();
	vec2 = ps_b.get_word_vector();

	vector<string>::iterator iter1 = vec1.begin();
	int count = 0;
	while(iter1 != vec1.end())
	{
		//find same word
		vector<string>::iterator iter = find_first_of(iter1, vec1.end(), vec2.begin(), vec2.end());
		if(iter != vec1.end())
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

int main(int argc, char *argv[])
{
	Config *p = Config::get_instance();
	string doc_offset;
	string pagelib_path;
	string dict_path;
	string model_path;
	//读取需要的文件
	p->get_file_name("doc_offset", doc_offset);
	p->get_file_name("pagelib_path", pagelib_path);
	p->get_file_name("dict_path", dict_path);
	p->get_file_name("model_path", model_path);

	//初始化切词工具
	CppJieba::MixSegment segment(dict_path, model_path);
	//create file stream
	ifstream infile;
	infile.open(doc_offset.c_str());
	ifstream inlib;
	inlib.open(pagelib_path.c_str());

	vector<PageSegment> _ps_vector;

	//把offset的索引文件读入vector中
	int docid, offset, length;
	while(infile >> docid >> offset >> length)
	{
		// cout << "-----------------" << endl;
		// cout << docid <<"\t" << offset << "\t" << length << endl;
		// cout << "-----------------" << endl;

		inlib.seekg(offset);
		char *buffer = new char[length + 1];
		inlib.read(buffer, length);
		buffer[length] = '\0';

		PageSegment ps;
		ps.doc_id = docid;
		ps.length = strlen(buffer);
		ps.content = string(buffer);

		ps.build_word_queue(buffer, segment);

		_ps_vector.push_back(ps);
	}

	//count same element
	int del_num = 0;
	for(vector<PageSegment>::size_type ix = 0; ix != _ps_vector.size() - 1; ++ix)
	{
		for(vector<PageSegment>::size_type iy = ix + 1; iy != _ps_vector.size(); ++iy)
		{
			#ifndef NDEBUG
			cout << "-------------------" << endl;
			cout << "ix : " << ix << " iy : " << iy << endl;
			cout << "-------------------" << endl;
			#endif
			//remove same page ix & ix + 1
			if(count_same_element(_ps_vector[ix], _ps_vector[iy]) > SIM_MIN_NUM)
			{
				cout << "-------------------" << endl;
				cout << "Delete a same page" << endl;
				cout << "-------------------" << endl;
				del_num ++;
				if(_ps_vector[ix].length > _ps_vector[iy].length)
				{
					_ps_vector[iy].set_del_status();
				}
				else
				{
					_ps_vector[ix].set_del_status();
				}
			}
		}
	}
	cout << "-------------------" << endl;
	cout << "Delete num : " << del_num << endl;
	cout << "-------------------" << endl;

	//write to a new file
	ofstream outfile;
	string delduplicate_lib;
	p->get_file_name("delduplicate_lib", delduplicate_lib);
	outfile.open(delduplicate_lib.c_str());
	//write to  delduplicate_lib
	for(vector<PageSegment>::size_type ix = 0; ix != _ps_vector.size(); ++ix)
	{
		//if del_tag is false
		if(!_ps_vector[ix].del_tag)
		{
			//rewrite docid;
			// size_t pos = _ps_vector[ix].content.find("<docid>");
			// _ps_vector[ix].content.replace(pos + strlen("<docid>"), 1, to_string(ix + 1));
			outfile << _ps_vector[ix].content << endl;
		}
	}

	outfile.close();
	infile.close();
	inlib.close();
	return 0;
}

