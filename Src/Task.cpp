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
	// int send_len = jsonstr.size();
	// send(_client_fd, &send_len, sizeof(send_len), 0);
	// int sum = 0;
	// while(sum < send_len)
	// {
	// 	int nsend = send(_client_fd, jsonstr.c_str() + sum, send_len - sum, 0);
	// 	sum += nsend;
	// }
	int sum = send(_client_fd, jsonstr.c_str(), jsonstr.size(), 0);
	cout << "send_size: " << sum << endl; 
}

//将string转换成json格式
string Task::json_string()
{
	Json::Value root;
	Json::Value arr;
	int count = 0;
	for(auto & x : _send_vec)
	{
		count ++;
		if(count == 20)
		{
			break;
		}
		Json::Value elem;
		// char title[1024] = "";
		// char summary[10240] = "";
		// strcpy(title, x.first.c_str());
		// strcpy(summary, x.second.c_str());
		// sprintf(title, "%s", title);
		// sprintf(summary, "sum\n%s", summary);
		elem["title"] = x._title;
		elem["summary"] = x._content;
		arr.append(elem);
	}
	root["files"] = arr;
	Json::StyledWriter stlwriter;
	return stlwriter.write(root);
}
