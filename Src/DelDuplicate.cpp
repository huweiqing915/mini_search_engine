/*************************************************************************
	> File Name: DelDuplicate.cpp
	> Author: huwq
	> Mail:huweiqing915@gmail.com 
	> Created Time: 2014年05月15日 星期四 22时07分22秒
 ************************************************************************/

#include "DelDuplicate.h"
#include <iostream>

using namespace std;

DelDuplicate::DelDuplicate()
{

}

DelDuplicate::~DelDuplicate()
{

}

static inline int max(int a, int b)
{
	return a > b ? a : b;
}

static inline int min(int a, int b)
{
	return a < b ? a : b; 
}

//建立含有特征码网页的vector
void DelDuplicate::build_page_vector()
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

		FeatureCode fc;
		fc._docid = docid;
		fc._length = strlen(buffer);
		fc._content = string(buffer);

		fc.build_feature_code();
		// fc.debug();
		_del_vector.push_back(fc);
	}
}

void DelDuplicate::delete_duplicate_page()
{
	cout << "-------------------" << endl;
	cout << "start delete pages" << endl;
	cout << "-------------------" << endl;

	int del_num = 0;
	string tmp;
	for(vector<FeatureCode>::size_type ix = 0; ix != _del_vector.size() - 1; ++ix)
	{
		tmp.clear();
		tmp = _del_vector[ix].get_feature_code();
		cout << "--------_del_vector[ix]--------" << ix << endl;
		if(_del_vector[ix]._del_tag == true)	//if is deleted,pass it
		{
			continue;
		}

		for(vector<FeatureCode>::size_type iy = ix + 1; iy != _del_vector.size(); ++iy)
		{
			if(_del_vector[ix]._del_tag == true)
			{
				break;
			}
			if(_del_vector[iy]._del_tag == true)
			{
				continue;
			}
			// cout << "-------------------" << endl;
			// cout << "ix : " << ix << " iy : " << iy << endl;
			// cout << "-------------------" << endl;
			//remove same page ix & ix + 1
			int lcs = longest_common_sequence(tmp, _del_vector[iy].get_feature_code());
			//len(LCS)/min(len(S1), len(S2))>0.3
			int min_len = min(_del_vector[ix].feature_code_size(), _del_vector[iy].feature_code_size());

			if(min_len < 10)
			{
				continue;
			}

			float result = (float)lcs / (float)min_len;

			// cout << "----------------" << endl;
			// cout << "Lcs: " << lcs << "min_len: " << min_len << endl; 
			// cout << "result" << result << endl;
			// cout << "----------------" << endl;

			if(result > SIM_MIN_NUM)
			{
				// cout << "-------------------" << endl;
				cout << "Delete a same page : " << del_num <<endl;
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
}

void DelDuplicate::parse_gbk_string(const string &str, vector<uint16_t> &vec) 
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

int DelDuplicate::longest_common_sequence(const string &str1, const string &str2)
{
	vector<uint16_t> vec1;
	vector<uint16_t> vec2;
	parse_gbk_string(str1, vec1);
	parse_gbk_string(str2, vec2);

	int len1 = vec1.size();
	int len2 = vec2.size();

	int **mem = new int *[len1 + 1];
	for(int k = 0; k <= len1; ++k)
	{
		mem[k] = new int[len2 + 1];
		bzero(mem[k], (len2+1)*sizeof(int));
	}
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

void DelDuplicate::write_to_file()
{
	ofstream outfile;
	string delduplicate_lib;
	Config *p = Config::get_instance();
	p->get_file_name("delduplicate_lib", delduplicate_lib);
	outfile.open(delduplicate_lib.c_str());
	//write to  delduplicate_lib
	for(vector<FeatureCode>::size_type ix = 0; ix != _del_vector.size(); ++ix)
	{
		//if del_tag is false
		if(!_del_vector[ix]._del_tag)
		{
			outfile << _del_vector[ix]._content << endl;
		}
	}
	outfile.close();
}
