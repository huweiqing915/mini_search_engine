/*************************************************************************
	> File Name: PageLib.h
	> Author: huwq
	> Mail:huweiqing915@gmail.com 
	> Created Time: 2014年05月15日 星期四 15时02分27秒
 ************************************************************************/

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

	void traversal_dir(const std::string &dirname);
	//title - content
	void get_title_content(const std::string &filename, std::string &, std::string &);
	//docid	- url - title - content	
	void write_lib(int, const std::string&, const std::string&, std::string&);

private:
};

#endif
