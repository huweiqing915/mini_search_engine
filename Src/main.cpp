/*************************************************************************
	> File Name: main.cpp
	> Author: huwq
	> Mail:huweiqing915@gmail.com 
	> Created Time: 2014年05月14日 星期三 19时09分25秒
 ************************************************************************/

#include "TCPSocket.h"
#include "Config.h"
#include "Task.h"

using namespace std;

int main(int argc, char *argv[])
{
	Config *p = Config::get_instance();
	string ip, port;
	p->get_file_name("server_ip", ip);
	p->get_file_name("server_port", port);
	TCPSocket server(ip, port);	//从配置文件读取ip和port
	int new_fd;
	pid_t pid;
	server.tcp_server_init();	//初始化server
	while(new_fd = server.tcp_accept())
	{
		cout << "client connect" << endl;
		pid = fork();
		if(pid == 0)	//pid = 0是子进程
		{
			char recv_buf[1024];
			int iret = server.recv_message(new_fd, (void*)recv_buf, 1024);
			Task task;
			task._client_fd = new_fd;
			task.excute_task();
			exit(1);
		}
		else if(pid > 0)	//pid>0是父进程
		{
			close(new_fd);
			waitpid(-1, NULL, WNOHANG);
		}
	}
	return 0;
}
