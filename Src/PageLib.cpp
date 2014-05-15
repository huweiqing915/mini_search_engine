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
			traversal_dir(entry->d_name); //递归
		}
		else //是文件
		{
			doc_id ++;	//记录docid
			char dir_buf[512];
			bzero(dir_buf, 512);
			getcwd(dir_buf, 512);
			char url[512];	//记录url
			sprintf(url, "%s/%s", dir_buf, entry->d_name);
			cout << url << endl;
			string title;
			string content;
			get_title_content(entry->d_name, title, content);
			write_lib(doc_id, url, title, content);
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

//取文章标题
void PageLib::get_title_content(const string &filename, string &title, string &content)
{
	ifstream infile;
	if(!open_file(infile,filename))
	{
		throw runtime_error("open file error!");
	}
	
	infile.seekg(0, infile.end);
	int length = infile.tellg();
	infile.seekg(0, infile.beg);

	char * buffer = new char [length];
	infile.read(buffer, length);

	content = string(buffer);
	if(infile)
	{
		cout << "--------------------" << endl;
		cout << "read success " << endl;
	}
	else
		cout << "error" << endl;

	infile.clear();
	infile.seekg(0, infile.beg);

	EncodingConverter trans;
	string cmp = trans.utf8_to_gbk("【 标  题 】");
	string line;
	for(int ix = 0; ix != 20; ++ix)
	{
		line.clear();
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
		infile.seekg(0, infile.beg);
		getline(infile, title);
	}
}


void PageLib::write_lib(int docid, const string &url, const string &title, string &content)
{
	string page = "<doc>\n<docid>" + to_string(docid) + "</docid>\n<url>" + url + "</url>\n<title>" + 
					title + "</title>\n<content>\n" + content + "\n</content>\n</doc>" + "\n";
	string pagelib_path;
	Config *p = Config::get_instance();
	p->get_file_name("pagelib_path", pagelib_path);
	ofstream outfile;
	outfile.open(pagelib_path, ofstream::out | ofstream::app);
	outfile << page << endl;
	outfile.close();
	cout << "write over!" << endl; 
}

