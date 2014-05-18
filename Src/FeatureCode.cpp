/*************************************************************************
	> File Name: FeatureCode.cpp
	> Author: huwq
	> Mail:huweiqing915@gmail.com 
	> Created Time: 2014年05月18日 星期日 11时15分41秒
 ************************************************************************/

#include "FeatureCode.h"
#include <iostream>

using namespace std;

FeatureCode::FeatureCode():_del_tag(false)
{
	
}

FeatureCode::~FeatureCode()
{

}

void FeatureCode::build_feature_code()
{
	//处理中文标点，转化成uint16_t 
	EncodingConverter trans;
	string cpunct = trans.utf8_to_gbk(CHINESE_PUNCT);
	uint16_t punct = (cpunct[0] << 8) + cpunct[1];

	//只能查找中文的标点的方法（注释部分）
	// int pos = 0;
	// while((pos = _content.find(punct, pos)) != string::npos)
	// {
	// 	cout << "------------------" << endl;
	// 	cout << pos << endl;
	// 	cout << "------------------" << endl;
	// 	 _feature_code += _content.substr(pos - 6, CUT_LEN);
	// 	++ pos;
	// }

	for(string::size_type ix = 0; ix != _content.size(); ++ix)
	{
		if((_content[ix] & 0x80))	//GBK
		{
			uint16_t tmp = (_content[ix] << 8) + _content[ix + 1];
			if(tmp == punct)	//if find Chinese punct ','
			{
				_feature_code += _content.substr(ix - 6, CUT_LEN);
			}
			++ix;
		}	
		else	//ASCII
		{
			if(_content[ix] == ENGLISH_PUNCT)	 //if find english punct ','
			{
				_feature_code += _content.substr(ix - 6, CUT_LEN - 1);
			}
		}
	}
}

const string& FeatureCode::get_feature_code()
{
	return _feature_code;
}

int FeatureCode::feature_code_size() const
{
	return _feature_code.size();
}

void FeatureCode::set_del_status()
{
	_del_tag = true;
}

void FeatureCode::debug()
{
	cout << _feature_code << endl;
}


