/*************************************************************************
	> File Name: test.cpp
	> Author: huwq
	> Mail:huweiqing915@gmail.com 
	> Created Time: 2014年05月19日 星期一 21时17分06秒
 ************************************************************************/

#include <iostream>
#include <unordered_map>
#include <utility>
#include <string>
#include <vector>
using namespace std;

int main()
{
	unordered_map<string, vector<pair<int, int> > > index;
	string word = "hello";
	int i = 1;
	int j = 2;
	int m = 3;
	int n = 4;
	vector<pair<int, int> > tmp;
	tmp.push_back(make_pair(i, j));
	tmp.push_back(make_pair(m, n));

	index[word] = tmp;
	for(auto & x : index)
	{
		cout << x.first << "\t";
		for(auto & y : x.second)
		{
			cout << y.first << " " << y.second << "\t";
		}
		cout << endl;
	}
	string search;
	cout << "search:" << endl;
	cin >> search;
	unordered_map<string, vector<pair<int, int> > >::iterator iter = index.find(search);
	if(iter != index.end())
	{
		cout << iter->first << " " << iter->second[0].first << " " << iter->second[0].second << endl;
	}
	
	return 0;
}
