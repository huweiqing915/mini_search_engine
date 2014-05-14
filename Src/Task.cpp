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
	int i = 0;
	while(i < 10)
	{
		Json::Value elem;
		char title[128] = "";
		char summary[128] = "";
		sprintf(title, "title_%d", i);
		sprintf(summary, "summary_%d", i);
		elem["title"] = title;
		elem["summary"] = summary;
		arr.append(elem);
		i++;
	}
	root["files"] = arr;
	Json::StyledWriter stlwriter;
	return stlwriter.write(root);
}
