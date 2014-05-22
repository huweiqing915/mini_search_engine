/*************************************************************************
	> File Name: RemoveRepeat.h
	> Author: huwq
	> Mail:huweiqing915@gmail.com 
	> Created Time: 2014年05月18日 星期日 17时06分21秒
 ************************************************************************/

#ifndef REMOVE_REPEAT_H_
#define REMOVE_REPEAT_H_

#include "Document.h"

class RemoveRepeat {
public:
	RemoveRepeat();
	~RemoveRepeat();

	void build_document_vec(); //把所有文档存入vector
	void remove_repeat_page();  //去除重复文档
	void write_to_file();	//将去重后的写入新的ripepage库
private:
	//所有文档都放在这个vector中，每个文档存有词频前几个的词
	std::vector<Document> _doc_vec; 
};

#endif

