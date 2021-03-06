/*************************************************************************
	> File Name: test_offset.cpp
	> Author: huwq
	> Mail:huweiqing915@gmail.com 
	> Created Time: 2014年05月16日 星期五 10时38分58秒
 ************************************************************************/

//测试offset.lib文件
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <string.h>
#include <ostream>
#include <stdio.h>

#include "EncodingConverter.h"	
#include "Config.h"

using namespace std;

int main()
{
	vector<string> index;
	Config *p = Config::get_instance();
	string doc_offset;
	string pagelib_path;
	//读取需要的文件
	p->get_file_name("new_offset", doc_offset);
	p->get_file_name("delduplicate_lib", pagelib_path);
	ifstream infile;
	infile.open(doc_offset.c_str());
	ifstream inlib;
	inlib.open(pagelib_path.c_str());

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
		//根据offset和length取出文档
		inlib.seekg(offset);
		char *buffer = new char[length + 1];
		inlib.read(buffer, length);
		buffer[length] = '\0';

	#ifndef NDEBUG
		ofstream outfile;
		outfile.open("a.txt");
		outfile << buffer << endl;
		outfile.close();
		outfile.clear();
	#endif	
		//test file offset
		// EncodingConverter trans;
		// string content = trans.gbk_to_utf8(buffer);
		cout << buffer << endl;

		delete [] buffer;
		buffer = NULL;
		cout << "-----------------------------" << endl;
		cout << "input docid" << endl;
	}
	infile.close();
	inlib.close();
	return 0;
}

