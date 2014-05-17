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
	string cpunct = trans.utf8_to_gbk("，");
	uint16_t punct = (cpunct[0] << 8) + cpunct[1];

	// cout << punct.size() << endl;
	//find punct(,)
	// int pos = 0;
	// while((pos = _content.find(punct, pos)) != string::npos)
	// {
	// 	cout << "------------------" << endl;
	// 	cout << pos << endl;
	// 	cout << "------------------" << endl;
	// 	 _feature_code += _content.substr(pos - 6, CUT_LEN);
	// 	// cout << _content.substr(pos - 6, CUT_LEN) << endl;
	// 	++ pos;
	// }
	for(string::size_type ix = 0; ix != _content.size(); ++ix)
	{
		//if is GBK
		if((_content[ix] & 0x80))
		{
			uint16_t tmp = (_content[ix] << 8) + _content[ix + 1];
			if(tmp == punct)
			{
				cout << "-----GBK-----" << endl;
				cout << ix << endl;
				cout << "-------------" << endl;
			}
			++ix;
		}
		else
		{
			if(_content[ix] == ',')	 //if find english punct ','
			{
				cout << "-----english----" << endl;
				cout << ix << endl;
				cout << "-------------" << endl;
			}
		}
	}
}

void DelDuplicate::debug()
{
	cout << _feature_code << endl;
}
