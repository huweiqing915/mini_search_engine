/*************************************************************************
	> File Name: Task.h
	> Author: huwq
	> Mail:huweiqing915@gmail.com 
	> Created Time: 2014年05月14日 星期三 19时39分49秒
 ************************************************************************/

#ifndef TASK_H_
#define TASK_H_

#include <string>

#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include "json/json.h"

class Task {
public:
	int _client_fd;
	void excute_task();
	std::string json_string();
private:
	std::string _message;
};

#endif
