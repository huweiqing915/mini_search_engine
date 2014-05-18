/*************************************************************************
	> File Name: test_delduplicate.cpp
	> Author: huwq
	> Mail:huweiqing915@gmail.com 
	> Created Time: 2014年05月16日 星期五 23时37分07秒
 ************************************************************************/

#include <iostream>
#include <fstream>
#include <vector>
#include "Config.h"
#include "DelDuplicate.h"

#define SIM_MIN_NUM 0.6

using namespace std;

void parse_gbk_string(const string &str, vector<uint16_t> &vec) 
{
    vec.clear();
    for (string::size_type ix = 0; ix != str.size(); ++ix) 
    {
        if (str[ix] & 0x80) 
        {
        	uint16_t t;
            if (ix + 1 == str.size()) //check
            {
                // throw runtime_error("invalid GBK string");
            	t = str[ix];
            	vec.push_back(t);
            	break;
            }
            else
            {
            	t = (str[ix] << 8) + str[ix + 1];
            	vec.push_back(t);
            	++ix;
            }       
        } 
        else 
        {
        	uint16_t t1 = str[ix];
            vec.push_back(t1);
        }
    }
}

inline int max(int a, int b)
{
	return a > b ? a : b;
}

inline int min(int a, int b)
{
	return a < b ? a : b; 
}

int longest_common_sequence(const string &str1, const string &str2)
{
	vector<uint16_t> vec1;
	vector<uint16_t> vec2;
	parse_gbk_string(str1, vec1);
	parse_gbk_string(str2, vec2);

	int len1 = vec1.size();
	int len2 = vec2.size();
#ifndef NDEBUG
	cout << "-------len1-------" << endl;
	cout << len1 << endl;
	cout << "--------len2--------" << endl;
	cout << len2 << endl;
	cout << "---------------------" << endl;
#endif
	int **mem = new int *[len1 + 1];
	for(int k = 0; k <= len1; ++k)
	{
		mem[k] = new int[len2 + 1];
	}
	// for(int i = 0; i != len1 + 1; ++i)
	// {
	// 	mem[i][0] = 0;
	// }
	// for(int j = 0; j != len2 + 1; ++j)
	// {
	// 	mem[0][j] = 0;
	// }
	//dp 
	for(int i = 1; i != len1 + 1; ++i)
	{
		for(int j = 1; j != len2 + 1; ++j)
		{
			if(vec1[i-1] == vec2[j-1])
			{
				mem[i][j] = mem[i-1][j-1] + 1;
			}
			else
			{
				mem[i][j] = max(mem[i-1][j], mem[i][j-1]);
			}
		}
	}
	int ret = mem[len1][len2];
	for(int k = 0; k <= len1; ++k)
	{
		delete [] mem[k];
	}
	delete [] mem;
	mem = NULL;

	return ret;
}

int main(int argc, char *argv[])
{
	Config *p = Config::get_instance();
	string doc_offset;
	string pagelib_path;
	//读取需要的文件
	p->get_file_name("doc_offset", doc_offset);
	p->get_file_name("pagelib_path", pagelib_path);

	//创建文件流
	ifstream infile;
	infile.open(doc_offset.c_str());
	ifstream inlib;
	inlib.open(pagelib_path.c_str());

	vector<DelDuplicate> _del_vector;

	//把offset的索引文件读入vector中
	int docid, offset, length;
	while(infile >> docid >> offset >> length)
	{
		// cout << "-----------------" << endl;
		// cout << docid <<"\t" << offset << "\t" << length << endl;
		// cout << "-----------------" << endl;
		
		inlib.seekg(offset);
		char *buffer = new char[length + 1];
		inlib.read(buffer, length);
		buffer[length] = '\0';

		DelDuplicate del;
		del._docid = docid;
		del._length = strlen(buffer);
		del._content = string(buffer);

		del.build_feature_code();
		// del.debug();

		_del_vector.push_back(del);
	}

	cout << "-------------------" << endl;
	cout << "start delete pages" << endl;
	cout << "-------------------" << endl;

	int del_num = 0;
	for(vector<DelDuplicate>::size_type ix = 0; ix != _del_vector.size() - 1; ++ix)
	{
		cout << "--------_del_vector[ix]--------" << ix << endl;
		if(_del_vector[ix]._del_tag == true)	//if is deleted,pass it
		{
			continue;
		}
		for(vector<DelDuplicate>::size_type iy = ix + 1; 
			(iy != _del_vector.size()) && (_del_vector[iy]._del_tag != true); ++iy)
		{
			// if(_del_vector[iy]._del_tag == true)
			// {
			// 	continue;
			// }
			// cout << "-------------------" << endl;
			// cout << "ix : " << ix << " iy : " << iy << endl;
			// cout << "-------------------" << endl;
			#ifndef NDEBUG
				cout << "--------_del_vector[ix]--------" << ix << endl;
				cout << _del_vector[ix].get_feature_code() << endl;
				cout << "-------_del_vector[iy]-------" << iy << endl;
				cout << _del_vector[iy].get_feature_code() << endl;
				cout << "-------ix size-----------" << endl; 
				cout << _del_vector[ix].get_feature_code().length() << endl;
				cout << "--------iy size-------" << endl; 
				cout << _del_vector[iy].get_feature_code().length() << endl;
				cout << "-------------------" << endl; 
			#endif
			//remove same page ix & ix + 1
			int lcs = longest_common_sequence(_del_vector[ix].get_feature_code(), 
						_del_vector[iy].get_feature_code());
			//len(LCS)/min(len(S1), len(S2))>0.3
			int min_len = min(_del_vector[ix].get_feature_code().length(), 
				_del_vector[iy].get_feature_code().length());
			if(min_len < 10)
			{
				continue;
			}
			float result = (float)lcs / (float)min_len;
			#ifndef NDEBUG
			cout << "----------------" << endl;
			cout << "Lcs: " << lcs << "min_len: " << min_len << endl; 
			cout << "result" << result << endl;
			cout << "----------------" << endl;
			#endif
			if(result > SIM_MIN_NUM)
			{
				// cout << "-------------------" << endl;
				// cout << "Delete a same page" << endl;
				// cout << "-------------------" << endl;
				del_num ++;
				if(_del_vector[ix]._length > _del_vector[iy]._length)
				{
					_del_vector[iy].set_del_status();
				}
				else
				{
					_del_vector[ix].set_del_status();
				}
			}
		}
	}
	cout << "-------------------" << endl;
	cout << "Delete num : " << del_num << endl;
	cout << "-------------------" << endl;

	ofstream outfile;
	string delduplicate_lib;
	p->get_file_name("delduplicate_lib", delduplicate_lib);
	outfile.open(delduplicate_lib.c_str());
	//write to  delduplicate_lib
	for(vector<DelDuplicate>::size_type ix = 0; ix != _del_vector.size(); ++ix)
	{
		//if del_tag is false
		if(!_del_vector[ix]._del_tag)
		{
			outfile << _del_vector[ix]._content << endl;
		}
	}

	outfile.close();
	infile.close();
	inlib.close();
	return 0;
}
