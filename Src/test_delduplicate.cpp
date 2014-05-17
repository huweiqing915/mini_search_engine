/*************************************************************************
	> File Name: test_delduplicate.cpp
	> Author: huwq
	> Mail:huweiqing915@gmail.com 
	> Created Time: 2014年05月16日 星期五 23时37分07秒
 ************************************************************************/

#include <iostream>
#include <vector>
#include "Config.h"
#include "DelDuplicate.h"

using namespace std;

int main(int argc, char *argv[])
{
	Config *p = Config::get_instance();
	string doc_offset;
	string pagelib_path;
	//读取需要的文件
	p->get_file_name("doc_offset", doc_offset);
	p->get_file_name("pagelib_path", pagelib_path);

	//创建文件流
	ifstream infile;
	infile.open(doc_offset.c_str());
	ifstream inlib;
	inlib.open(pagelib_path.c_str());

	vector<DelDuplicate> _del_vector;

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

		DelDuplicate del;
		del._docid = docid;
		del._length = strlen(buffer);
		del._content = string(buffer);

		del.build_feature_code();
		// del.debug();

		_del_vector.push_back(del);
	}


	infile.close();
	inlib.close();
	return 0;
}
