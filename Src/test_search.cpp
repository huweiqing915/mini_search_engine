/*************************************************************************
	> File Name: test_search.cpp
	> Author: huwq
	> Mail:huweiqing915@gmail.com 
	> Created Time: 2014年05月19日 星期一 16时34分58秒
 ************************************************************************/

#include <iostream>
#include "Config.h"
#include "Search.h"
#include "MixSegment.hpp"
using namespace std;

int main(int argc, char const *argv[])
{
	Config *p = Config::get_instance();
	string dict_path;
	string model_path;
	//读取需要的文件
	p->get_file_name("dict_path", dict_path);
	p->get_file_name("model_path", model_path);

	vector<pair<string, string> > result_vec;

	//初始化切词工具
	CppJieba::MixSegment segment(dict_path, model_path);

	Search query;
	string word;
	cout << "input word:" << endl;
	while(cin >> word)
	{
		result_vec.clear();
		query.search_result(word, result_vec, segment);
		for(auto & x: result_vec)
		{
			cout << x.first << endl;
			cout << x.second << endl;
			cout << "---------------" << endl;
		}
	}

	return 0;
}
