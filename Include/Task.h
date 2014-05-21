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
#include <vector>
#include <utility>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include "json/json.h"

class Task {
public:
	int _client_fd;
	std::vector<std::pair<std::string, std::string> > _send_vec;
	void excute_task();
	std::string json_string();
private:
	std::string _message;
};

#endif
