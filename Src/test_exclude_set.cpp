/*************************************************************************
	> File Name: test_exclude_set.cpp
	> Author: huwq
	> Mail:huweiqing915@gmail.com 
	> Created Time: 2014年05月15日 星期四 23时18分51秒
 ************************************************************************/

#include "ExcludeSet.h"
#include <iostream>

using namespace std;

int main()
{
	ExcludeSet *p = ExcludeSet::get_instance();
	set<string> exclude = p->get_exclude_set();
	for(auto & x : exclude)
	{
		cout << x << endl;
	}
	return 0;
}
