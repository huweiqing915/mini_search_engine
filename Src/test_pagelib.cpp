/*************************************************************************
	> File Name: test_pagelib.cpp
	> Author: huwq
	> Mail:huweiqing915@gmail.com 
	> Created Time: 2014年05月15日 星期四 19时45分46秒
 ************************************************************************/

#include "PageLib.h"
#include "Config.h"

using namespace std;

int main()
{
	Config *p_config = Config::get_instance();
	string pagelib_path;
	string doc_offset;
	string dir_name;
	p_config->get_file_name("pagelib_path", pagelib_path);
	p_config->get_file_name("doc_offset", doc_offset);
	p_config->get_file_name("dir_name", dir_name);
	//清空原来的文档
	ofstream outfile(pagelib_path.c_str(), ofstream::out | ofstream::trunc);
	ofstream outfile1(doc_offset.c_str(), ofstream::out | ofstream::trunc);
	outfile.close();
	outfile1.close();

	PageLib pagelib;
	pagelib.traversal_dir(dir_name);
	return 0;
}
