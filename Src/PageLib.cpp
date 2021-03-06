/*************************************************************************
	> File Name: PageLib.cpp
	> Author: huwq
	> Mail:huweiqing915@gmail.com 
	> Created Time: 2014年05月15日 星期四 15时02分20秒
 ************************************************************************/

#include "PageLib.h"
#include "Config.h"

using namespace std;

int doc_id = 0;

PageLib::PageLib()
{

}

PageLib::~PageLib()
{

}

//遍历目录
void PageLib::traversal_dir(const string &dirname)
{
	DIR *pDir;
	struct dirent *entry;
	struct stat stat_buf;
	if((pDir = opendir(dirname.c_str())) == NULL)
	{
		throw runtime_error("open directory error!");
	}
	//切换到当前目录
	chdir(dirname.c_str());
	while((entry = readdir(pDir)) != NULL)
	{
		stat(entry->d_name, &stat_buf);
		if(S_ISDIR(stat_buf.st_mode))	//文件类型是目录
		{
			if(strcmp(".", entry->d_name) == 0 || strcmp("..", entry->d_name) == 0)
			{
				continue;
			}
			traversal_dir(entry->d_name); //递归遍历
		}
		else //如果是文件
		{
			doc_id ++;	//记录docid
			char dir_buf[512];
			bzero(dir_buf, 512);
			getcwd(dir_buf, 512);
			char url[512];	//记录url
			sprintf(url, "%s/%s", dir_buf, entry->d_name);
			cout << url << endl;
			string title; //记录文档标题
			string content;	//记录文档内容
			int offset, length; //记录文档偏移，及文档大小
			get_title_content(entry->d_name, title, content);
			write_lib(doc_id, url, title, content, offset, length);
			build_offset_index(doc_id, offset, length);
			title.clear();
			content.clear();
		}
	}
	chdir("..");	//切换到上级目录
	closedir(pDir);
}

static ifstream& open_file(ifstream &is, const string &filename)
{
	is.close();
	is.clear();
	is.open(filename.c_str());
	return is;
}

//取每一篇文章标题和内容
void PageLib::get_title_content(const string &filename, string &title, string &content)
{
	ifstream infile;
	if(!open_file(infile,filename))
	{
		throw runtime_error("open file error!");
	}
	
	infile.seekg(0, infile.end);	//把文件流定位到文件尾
	int length = infile.tellg();	//返回当前文件尾的位置,就可以获得文档长度
	infile.seekg(0, infile.beg);	//将文件流返回文件头

	char * buffer = new char [length + 1];
	infile.read(buffer, length);	//读取文件的内容
	buffer[length] = '\0';
	content = string(buffer);

	delete [] buffer;	//释放申请的内存空间，防止内存泄露
	buffer = NULL;
#ifndef NDEBUG
	if(infile)
	{
		cout << "--------------------" << endl;
		cout << "read success " << endl;
	}
	else
		cout << "error" << endl;
#endif

	infile.clear();
	infile.seekg(0, infile.beg);	//再次返回文件头，为了取标题

	EncodingConverter trans;
	string cmp = trans.utf8_to_gbk("【 标  题 】");	//取标题
	for(int ix = 0; ix != 20; ++ix)	//超过20行就不找了
	{
		string line;
		getline(infile, line);
		size_t found = line.find(cmp);
		if(found != string::npos) //如果找到了标题
		{
			title = line.substr(found + cmp.size(), line.size()); 
			break;
		}
	}
	
	if(title.empty())	//如果没找到，就取第一行
	{
		infile.seekg(0, infile.beg);	//返回文件头，取第一行
		getline(infile, title);
	}

}


void PageLib::write_lib(int docid, const string &url, const string &title, string &content, int &offset,int &length)
{
	string page = "<doc>\n<docid>" + to_string(docid) + "</docid>\n<url>" + url + "</url>\n<title>" + 
					title + "</title>\n<content>\n" + content + "\n</content>\n</doc>\n"; //格式化
	string pagelib_path;
	Config *p = Config::get_instance();
	p->get_file_name("pagelib_path", pagelib_path);
	ofstream outfile;
	outfile.open(pagelib_path.c_str(), ofstream::out | ofstream::app);  //追加的方式写入文件

	offset = outfile.tellp();	//文档的起始位置，即偏移量
	outfile << page << endl;	//写入文件
	int offset2 = outfile.tellp();	
	length = offset2 -offset;	//写完后的位置减去写入前的位置就是长度

	outfile.close();
#ifndef NDEBUG
	cout << "write lib over!" << endl; 
#endif
}

void PageLib::build_offset_index(int docid, int offset, int length)
{
	ofstream outfile;
	Config *p = Config::get_instance();
	string doc_offset;
	p->get_file_name("doc_offset", doc_offset);
	outfile.open(doc_offset.c_str(), ofstream::out | ofstream::app);
	//写入docid-offse-length的索引
	outfile << docid << "\t" << offset << "\t" << length << endl;

	outfile.close();
}
