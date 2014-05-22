/*************************************************************************
	> File Name: RemoveRepeat.cpp
	> Author: huwq
	> Mail:huweiqing915@gmail.com 
	> Created Time: 2014年05月18日 星期日 17时08分28秒
 ************************************************************************/

#include "RemoveRepeat.h"
#include "Config.h"

using namespace std;

RemoveRepeat::RemoveRepeat()
{

}

RemoveRepeat::~RemoveRepeat()
{

}

//把所有的document放入vector中，每一篇文档都是一个Document对象
void RemoveRepeat::build_document_vec()
{
	Config *p = Config::get_instance();
	string doc_offset;
	string pagelib_path;
	string dict_path;
	string model_path;
	//读取需要的文件
	p->get_file_name("doc_offset", doc_offset);
	p->get_file_name("pagelib_path", pagelib_path);
	p->get_file_name("dict_path", dict_path);
	p->get_file_name("model_path", model_path);

	//初始化切词工具
	CppJieba::MixSegment segment(dict_path, model_path);

	ifstream infile;
	infile.open(doc_offset.c_str());
	ifstream inlib;
	inlib.open(pagelib_path.c_str());

	int docid, offset, length;
	cout << "start" << endl;
	//读每一篇文档，放入vector中
	while(infile >> docid >> offset >> length)
	{
		cout << "-----------------" << endl;
		cout << docid <<"\t" << offset << "\t" << length << endl;
		cout << "-----------------" << endl;
		
		inlib.seekg(offset);
		char *buffer = new char[length + 1];
		inlib.read(buffer, length);
		buffer[length] = '\0';

		Document doc;
		doc._doc_id = docid;	//文档id
		doc._length = strlen(buffer);	//文档长度
		doc._content = string(buffer);  //文档内容

		doc.build_word_queue(buffer, segment);

		_doc_vec.push_back(doc);
	}

	infile.close();
	inlib.close();
}

//去除重复的文档
void RemoveRepeat::remove_repeat_page()
{
	int del_num = 0;
	for(vector<Document>::size_type ix = 0; ix != _doc_vec.size() - 1; ++ix)
	{
		cout << "_doc_vec[ix] : " << ix << endl;
		//如果已经删除就跳过
		if(_doc_vec[ix]._del_tag == true)
		{
			continue;
		}
		for(vector<Document>::size_type iy = ix + 1; iy != _doc_vec.size(); ++iy)
		{
			//已经删除就跳过
			if(_doc_vec[ix]._del_tag == true)
			{
				break;	//break to up level(for ix);
			}
			if(_doc_vec[iy]._del_tag == true)
			{
				continue;
			}
			#ifndef NDEBUG
				cout << "-------------------" << endl;
				cout << "ix : " << ix << " iy : " << iy << endl;
				cout << "-------------------" << endl;
			#endif
			//remove same page ix & iy
			if(_doc_vec[ix] == _doc_vec[iy])	//如果两个文档相同则删除
			{
			#ifndef NDEBUG
				cout << "-------------------" << endl;
				cout << "Delete a same page" << endl;
				cout << "-------------------" << endl;
			#endif
				del_num ++;
				//删除长度短的
				if(_doc_vec[ix]._length > _doc_vec[iy]._length)
				{
					_doc_vec[iy].set_del_status();
				}
				else
				{
					_doc_vec[ix].set_del_status();
				}
			}
		}
	}
	cout << "-------------------" << endl;
	cout << "Delete num : " << del_num << endl;
	cout << "-------------------" << endl;
}

//write to a new file
void RemoveRepeat::write_to_file()
{
	Config *p = Config::get_instance();
	ofstream outfile;
	ofstream ofs;
	string delduplicate_lib;
	string new_offset;
	p->get_file_name("delduplicate_lib", delduplicate_lib);
	p->get_file_name("new_offset", new_offset);

	outfile.open(delduplicate_lib.c_str());
	ofs.open(new_offset.c_str());
	int docid = 1;
	//write to  delduplicate_lib
	for(vector<Document>::size_type ix = 0; ix != _doc_vec.size(); ++ix)
	{
		//如果没有删除，就写入文件中
		if(!_doc_vec[ix]._del_tag)
		{
			//rewrite docid;
			size_t pos = _doc_vec[ix]._content.find("<docid>");
			size_t pos1 = _doc_vec[ix]._content.find("</docid>");
			size_t len = strlen("<docid>");
			//把docid换成新的，方便查询
			_doc_vec[ix]._content.replace(pos + len, pos1 - pos - len, to_string(docid));
			
			//计算偏移量和文档长度
			int offset = outfile.tellp();
			outfile << _doc_vec[ix]._content << endl;
			int offset1 = outfile.tellp();
			int length = offset1 - offset;

			ofs << docid << "\t" << offset << "\t" << length << endl;
			docid ++; 
		}
	}
	ofs.close();
	outfile.close();
}

