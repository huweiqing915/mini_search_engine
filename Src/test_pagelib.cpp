/*************************************************************************
	> File Name: test_pagelib.cpp
	> Author: huwq
	> Mail:huweiqing915@gmail.com 
	> Created Time: 2014年05月15日 星期四 19时45分46秒
 ************************************************************************/

#include "PageLib.h"

using namespace std;

int main()
{
	PageLib pagelib;
	string dirname = "../tmp/data";
//	string filename = "../tmp/test.txt";
//	string title;
//	string content;
//	int id = 1;
//	string url = "/tmp/test.txt";
//	pagelib.get_title_content(filename, title, content);
//	pagelib.write_lib(id, url, title, content);
	pagelib.traversal_dir(dirname);
	return 0;
}
