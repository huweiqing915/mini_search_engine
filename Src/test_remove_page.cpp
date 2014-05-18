/*************************************************************************
	> File Name: test_remove_page.cpp
	> Author: huwq
	> Mail:huweiqing915@gmail.com 
	> Created Time: 2014年05月16日 星期五 19时21分38秒
 ************************************************************************/

#include <iostream>
#include <fstream>
#include "Config.h"
#include "RemoveRepeat.h"

using namespace std;

int main(int argc, char *argv[])
{
	Config *p = Config::get_instance();
	string dict_path;
	string model_path;
	//读取需要的文件
	p->get_file_name("dict_path", dict_path);
	p->get_file_name("model_path", model_path);

	//初始化切词工具
	CppJieba::MixSegment segment(dict_path, model_path);

	cout << "start" << endl;
	RemoveRepeat rm;
	rm.build_document_vec(segment);
	rm.remove_repeat_page();
	rm.write_to_file();
	
	return 0;
}
