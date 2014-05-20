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

void RemoveRepeat::build_document_vec(const CppJieba::MixSegment &segment)
{
	Config *p = Config::get_instance();
	string doc_offset;
	string pagelib_path;
	p->get_file_name("doc_offset", doc_offset);
	p->get_file_name("pagelib_path", pagelib_path);

	ifstream infile;
	infile.open(doc_offset.c_str());
	ifstream inlib;
	inlib.open(pagelib_path.c_str());

	int docid, offset, length;
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
		doc.doc_id = docid;
		doc.length = strlen(buffer);
		doc.content = string(buffer);

		doc.build_word_queue(buffer, segment);

		_doc_vec.push_back(doc);
	}

	infile.close();
	inlib.close();
}

void RemoveRepeat::remove_repeat_page()
{
	int del_num = 0;
	for(vector<Document>::size_type ix = 0; ix != _doc_vec.size() - 1; ++ix)
	{
		cout << "_doc_vec[ix] : " << ix << endl;
		if(_doc_vec[ix].del_tag == true)
		{
			continue;
		}
		for(vector<Document>::size_type iy = ix + 1; iy != _doc_vec.size(); ++iy)
		{
			if(_doc_vec[ix].del_tag == true)
			{
				break;	//break to up level(for ix);
			}
			if(_doc_vec[iy].del_tag == true)
			{
				continue;
			}
			#ifndef NDEBUG
				cout << "-------------------" << endl;
				cout << "ix : " << ix << " iy : " << iy << endl;
				cout << "-------------------" << endl;
			#endif
			//remove same page ix & ix + 1
			if(_doc_vec[ix] == _doc_vec[iy])	//if two document is equal
			{
			#ifndef NDEBUG
				cout << "-------------------" << endl;
				cout << "Delete a same page" << endl;
				cout << "-------------------" << endl;
			#endif
				del_num ++;
				//删除长度短的
				if(_doc_vec[ix].length > _doc_vec[iy].length)
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
	int offset;
	int length;
	int docid = 1;
	//write to  delduplicate_lib
	for(vector<Document>::size_type ix = 0; ix != _doc_vec.size(); ++ix)
	{
		//if del_tag is false
		if(!_doc_vec[ix].del_tag)
		{
			//rewrite docid;
			size_t pos = _doc_vec[ix].content.find("<docid>");
			size_t pos1 = _doc_vec[ix].content.find("</docid>");
			size_t len = strlen("<docid>");
			_doc_vec[ix].content.replace(pos + len, pos1 - pos - len, to_string(docid));
			offset = outfile.tellp();
			outfile << _doc_vec[ix].content << endl;
			int offset1 = outfile.tellp();
			length = offset1 - offset;

			ofs << docid << "\t" << offset << "\t" << length << endl;
			docid ++; 
		}
	}
	ofs.close();
	outfile.close();
}

