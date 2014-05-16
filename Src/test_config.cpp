/*************************************************************************
	> File Name: test_config.cpp
	> Author: huwq
	> Mail:huweiqing915@gmail.com 
	> Created Time: 2014年05月16日 星期五 10时35分43秒
 ************************************************************************/

#include <iostream>
#include "Config.h"
using namespace std;

int main()
{
	string pagelib_path;
	Config *p = Config::get_instance();
	p->get_file_name("pagelib_path", pagelib_path);
	cout << pagelib_path << endl;
	return 0;
}
