/*************************************************************************
	> File Name: PageLib.h
	> Author: huwq
	> Mail:huweiqing915@gmail.com 
	> Created Time: 2014年05月15日 星期四 15时02分27秒
 ************************************************************************/
/*
 *	根据文档材料，建立网页库
 */

#ifndef PAGE_LIB_H_
#define PAGE_LIB_H_

#include <string>
#include <iostream>

#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>

#include "Config.h"
#include "EncodingConverter.h"

class PageLib {
public:
	PageLib();
	~PageLib();

	void traversal_dir(const std::string &dirname); //遍历目录
	//title - content
	void get_title_content(const std::string &filename, std::string &, std::string &);

private:
	//docid	- url - title - content	- offset - size
	void write_lib(int, const std::string&, const std::string&, std::string&, int &, int &);
	void build_offset_index(int docid, int offset, int length);	//建立文档id，偏移量（即文件中的起始位置），文章大小的索引
};

#endif
