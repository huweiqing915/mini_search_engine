/*************************************************************************
	> File Name: a.cpp
	> Author: huwq
	> Mail:huweiqing915@gmail.com 
	> Created Time: 2014年05月17日 星期六 20时28分38秒
 ************************************************************************/

#include <iostream>
using namespace std;

int main()
{
	string a = "，";
	cout << a.length() << endl;
	for(string::size_type ix = 0; ix != a.size(); ++ix)
	{
		if(a[ix] == '，')
		{
			cout << ix << endl;
		}
	}
}
