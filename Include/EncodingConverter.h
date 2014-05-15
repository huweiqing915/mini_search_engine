/*************************************************************************
	> File Name: EncodingConverter.h
	> Author: huwq
	> Mail:huweiqing915@gmail.com 
	> Created Time: 2014年05月15日 星期四 19时04分47秒
 ************************************************************************/

#ifndef ENCODINGCONVERTER_H_
#define ENCODINGCONVERTER_H_

#include <string>
#include <stdexcept>
#include <cstring>
#include <stdlib.h>
#include "iconv.h"

#define OUTLEN 255

class EncodingConverter
{
public:
	EncodingConverter();
	virtual ~EncodingConverter();

	std::string gbk_to_utf8(const std::string&);
	std::string utf8_to_gbk(const std::string&);

private:
	int code_convert(const char *from_charset, const char *to_charset, char *inbuf,
				int inlen, char *outbuf, int outlen);
};

#endif

