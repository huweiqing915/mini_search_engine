/*************************************************************************
	> File Name: DelDuplicate.cpp
	> Author: huwq
	> Mail:huweiqing915@gmail.com 
	> Created Time: 2014年05月15日 星期四 22时07分22秒
 ************************************************************************/

#include "DelDuplicate.h"

using namespace std;

DelDuplicate::DelDuplicate():_del_tag(false)
{
	
}

DelDuplicate::~DelDuplicate()
{

}

void DelDuplicate::build_feature_code()
{
	EncodingConverter trans;
	string punct = trans.utf8_to_gbk("，");
	//find punct(,)
	int pre_pos = 0;
	int pos = 0;
	while((pos = _content.find_first_of(punct, pos)) != string::npos)
	{
		cout << "------------------" << endl;
		cout << pos << endl;
		cout << "------------------" << endl;
		//if length < 6
		if(pos - pre_pos < 6)
		{
			pre_pos = pos;
			++ pos;
			continue;
		}
		_feature_code += _content.substr(pos - 6, pos + 6);
		pre_pos = pos;
		++ pos;
	}
}

void DelDuplicate::debug()
{
	cout << _feature_code << endl;
}