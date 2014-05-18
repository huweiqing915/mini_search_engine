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

	void build_document_vec(const CppJieba::MixSegment &);
	void remove_repeat_page();
	void write_to_file();
private:
	std::vector<Document> _doc_vec;
};

#endif

