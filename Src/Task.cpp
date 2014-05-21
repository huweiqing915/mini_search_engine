/*************************************************************************
	> File Name: Task.cpp
	> Author: huwq
	> Mail:huweiqing915@gmail.com 
	> Created Time: 2014年05月14日 星期三 19时39分43秒
 ************************************************************************/

#include "Task.h"

using namespace std;

void Task::excute_task()
{
	string jsonstr = json_string();
	send(_client_fd, jsonstr.c_str(), jsonstr.size(), 0);
}

//将string转换成json格式
string Task::json_string()
{
	Json::Value root;
	Json::Value arr;
	for(auto & x : _send_vec)
	{
		Json::Value elem;
		// char title[1024] = "";
		// char summary[10240] = "";
		// strcpy(title, x.first.c_str());
		// strcpy(summary, x.second.c_str());
		// sprintf(title, "%s", title);
		// sprintf(summary, "sum\n%s", summary);
		elem["title"] = x.first;
		elem["summary"] = x.second;
		arr.append(elem);
	}
	root["files"] = arr;
	Json::StyledWriter stlwriter;
	return stlwriter.write(root);
}
