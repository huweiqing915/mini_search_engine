/*************************************************************************
	> File Name: test_index.cpp
	> Author: huwq
	> Mail:huweiqing915@gmail.com 
	> Created Time: 2014年05月19日 星期一 00时22分53秒
 ************************************************************************/

#include <iostream>
#include "InvertIndex.h"
#include "Config.h"
#include "MixSegment.hpp"

using namespace std;

int main()
{
	Config *p = Config::get_instance();
	string dict_path;
	string model_path;
	//读取需要的文件
	p->get_file_name("dict_path", dict_path);
	p->get_file_name("model_path", model_path);

	//初始化切词工具
	CppJieba::MixSegment segment(dict_path, model_path);
	
	InvertIndex ix;
	ix.build_invert_index(segment);
	ix.count_word_weight();
	ix.write_word_weight();
	ix.write_index();

	return 0;
}
