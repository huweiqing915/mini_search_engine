/*************************************************************************
	> File Name: main.cpp
	> Author: huwq
	> Mail:huweiqing915@gmail.com 
	> Created Time: 2014年05月14日 星期三 19时09分25秒
 ************************************************************************/

#include "TCPSocket.h"
#include "Config.h"
#include "Task.h"
#include "Search.h"
#include <signal.h>

using namespace std;

// void wait_child_sign(int singno)
// {
// 	pid_t exit_pid = waitpid(-1, NULL, WNOHANG);
// 	if(exit_pid > 0)
// 	{
// 		cout << "Singal num:" << singno << " child " << exit_pid << " exit." << endl;
// 	}
// 	else if(exit_pid == 0)
// 	{
// 		cout << "no child exit!" << endl;
// 	}
// }

int main(int argc, char *argv[])
{
	Config *p = Config::get_instance();
	string ip, port;
    string dict_path;
    string model_path;
    //读取需要的文件
	p->get_file_name("server_ip", ip);
	p->get_file_name("server_port", port);
    p->get_file_name("dict_path", dict_path);
    p->get_file_name("model_path", model_path);

    //初始化切词工具
    CppJieba::MixSegment segment(dict_path, model_path);
    cout << "Overload segment done !" << endl;
    Search search;
   
    TCPSocket server(ip, port); //从配置文件读取ip和port
    server.tcp_server_init();   //初始化server--create socket(),bind(),listen(),setnonblock()

    int new_fd;
    
	while(new_fd = server.tcp_accept())
	{
		cout << "client connect" << endl;
	//	signal(SIGCHLD, wait_child_sign);
		pid_t pid = fork();
		if(pid == 0)	//pid = 0是子进程
		{
			vector<Document> result_vec;
			char recv_buf[1024];
			int iret = server.recv_message(new_fd, recv_buf, 1024);
			result_vec.clear();

			search.search_result(recv_buf, result_vec, segment);
			Task task;
			task._client_fd = new_fd;
            task._send_vec = result_vec;
            cout << "--------------" << endl;
            cout << "_send_vec.size():" << task._send_vec.size() << endl;
			task.excute_task();		
			close(task._client_fd);
			exit(1);
		}
		else if(pid > 0)	//pid>0是父进程
		{
			close(new_fd);
			pid_t exit_pid = waitpid(-1, NULL, WNOHANG);
			if(exit_pid > 0)
			{
				cout << exit_pid << " child exit !" << endl; 
			}
			else if(exit_pid == 0)
			{
				cout << "no child exit!" << endl;
			}
		}
	}
	return 0;
}
