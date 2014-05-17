/*************************************************************************
	> File Name: test_offset.cpp
	> Author: huwq
	> Mail:huweiqing915@gmail.com 
	> Created Time: 2014年05月16日 星期五 10时38分58秒
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
	//读取需要的文件
	p->get_file_name("doc_offset", doc_offset);
	p->get_file_name("pagelib_path", pagelib_path);
	ifstream infile;
	infile.open(doc_offset.c_str());
	ifstream inlib;
	inlib.open(pagelib_path.c_str());

	string line;
	int docid, offset, length;
	while(getline(infile, line))
	{
		index.push_back(line);
	}
	
	cout << "input docid" << endl;
	while(cin >> docid)
	{
		int id;
		line = index[docid - 1];
		istringstream str_stm(line);
		str_stm >> id >> offset >> length;
		//putout result
		cout << "----------------" << endl;
		cout << id << "\t" << offset << "\t" << length << endl;
		cout << "------------------" << endl;

		inlib.seekg(offset);
		char *buffer = new char[length + 1];
		inlib.read(buffer, length);
		buffer[length] = '\0';
		// PageSegment ps;
		// ps.build_word_queue(buffer);

		// for(int ix = 0; ix != 10; ix++)
		// {
		// 	string s = ps.get_top_word();
		// 	if(s[0] & 0x80)
		// 	{
		//  		cout << s << endl;
		//  	}
		// }
	#ifndef NDEBUG
		ofstream outfile;
		outfile.open("a.txt");
		cout << buffer << endl;
		outfile << buffer << endl;
		outfile.close();
		outfile.clear();
	#endif	
		delete [] buffer;
		cout << "-----------------------------" << endl;
		cout << "input docid" << endl;
	}
	infile.close();
	inlib.close();
	return 0;
}

