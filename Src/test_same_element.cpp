/*************************************************************************
	> File Name: test_same_element.cpp
	> Author: huwq
	> Mail:huweiqing915@gmail.com 
	> Created Time: 2014年05月16日 星期五 17时57分43秒
 ************************************************************************/

#include <vector>
#include <iostream>
#include <algorithm>
#include <string>

using namespace std;

int main()
{
	vector<string> vec1;
	vector<string> vec2;
	//result 3
	vec1.push_back("你好");
	vec1.push_back("中国");
	vec1.push_back("王道");
	vec1.push_back("考研");
	vec1.push_back("知道");
	vec1.push_back("下载");

	vec2.push_back("下载");
	vec2.push_back("博客");
	vec2.push_back("网络");
	vec2.push_back("考研");
	vec2.push_back("内容");
	vec2.push_back("你好");

	vector<string>::iterator iter1 = vec1.begin();
	int count = 0;

	while(iter1 != vec1.end())
	{
		vector<string>::iterator iter = find_first_of(iter1, vec1.end(), vec2.begin(), vec2.end());
		if(iter != vec1.end())
		{
			++count;
			iter1 = ++iter;
		}
		else 
		{
			break;
		}
	}

	cout << count << endl;
	return 0;
}

