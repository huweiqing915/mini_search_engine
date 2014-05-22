/*************************************************************************
	> File Name: test_pagesegment.cpp
	> Author: huwq
	> Mail:huweiqing915@gmail.com 
	> Created Time: 2014年05月21日 星期三 21时48分44秒
 ************************************************************************/

//测试offset.lib文件，测试PageSegment类
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <string.h>
#include <ostream>
#include <stdio.h>

#include "Config.h"
#include "PageSegment.h"

using namespace std;

int main()
{
	vector<string> index;
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
	ifstream infile;
	infile.open(doc_offset.c_str());
	ifstream inlib;
	inlib.open(pagelib_path.c_str());

	CppJieba::MixSegment segment(dict_path, model_path);

	string line;
	while(getline(infile, line))
	{
		index.push_back(line);
	}
	
	cout << "input docid" << endl;
	int input_id;
	while(cin >> input_id)
	{
		int docid, offset, length;
		line = index[input_id - 1];
		istringstream str_stm(line);
		str_stm >> docid >> offset >> length;
		//putout result
		cout << "----------------" << endl;
		cout << docid << "\t" << offset << "\t" << length << endl;
		cout << "------------------" << endl;

		inlib.seekg(offset);
		char *buffer = new char[length + 1];
		inlib.read(buffer, length);
		buffer[length] = '\0';

		//for test PageSegment

		PageSegment ps;
		ps.build_word_queue(buffer, segment);

		vector<string> word;
		word = ps.get_word_vector();
		//output
		for(auto & x : word)
		{
			cout << x << endl;
		}

		delete [] buffer;
		buffer = NULL;
		cout << "-----------------------------" << endl;
		cout << "input docid" << endl;
	}
	infile.close();
	inlib.close();
	return 0;
}

