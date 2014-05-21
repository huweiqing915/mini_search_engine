/*************************************************************************
	> File Name: server.cpp
	> Author: huwq
	> Mail:huweiqing915@gmail.com 
	> Created Time: 2014年05月15日 星期四 10时36分02秒
 ************************************************************************/

#include "TCPSocket.h"
#include "Search.h"
#include "Task.h"

#include <sys/epoll.h>

using namespace std;

#define TIMEOUT 120 //set epoll wait timeout
#define MAX_EVENTS 20

int main()
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

    Search search;
    vector<pair<string, string> > result_vec;

    TCPSocket server(ip, port); //从配置文件读取ip和port
    server.tcp_server_init();   //初始化server--create socket(),bind(),listen(),setnonblock()

	struct epoll_event ev, events[MAX_EVENTS];
	int epollfd = epoll_create(256);
	if (epollfd == -1)
    {
        throw runtime_error("epoll_create");
    }

    //设置与要处理的事件相关的文件描述符
    int listenfd = server.get_fd();
    ev.data.fd = listenfd;
    //设置要处理的事件类型，当描述符可读时触发，触发方式为ET模式
    ev.events = EPOLLIN | EPOLLET;
    //注册epoll事件
    epoll_ctl(epollfd, EPOLL_CTL_ADD, listenfd, &ev);

	while(true)
	{
		int nfds = epoll_wait(epollfd, events, MAX_EVENTS, TIMEOUT);
		// cout << nfds << endl;
		sleep(2);
        if (nfds == -1)
        {
            throw runtime_error("epoll wait error!");
        }
        //处理所发生的所有事件
        for (int ix = 0; ix < nfds; ++ix)
        {
        	//如果新监测到一个SOCKET用户连接到了绑定的SOCKET端口，建立新的连接。
            if (events[ix].data.fd == listenfd)
            {
                int new_fd = server.tcp_accept();
                server.set_non_blocking(new_fd);	//设置为非阻塞方式
                ev.events = EPOLLIN | EPOLLET;
                //设置用于读操作的文件描述符
                ev.data.fd = new_fd;
                //设置用于注册的读操作事件
                //EPOLLIN ：表示对应的文件描述符可以读,ET模型
                ev.events = EPOLLIN | EPOLLET;	
                //注册ev，每次有新的客户端的连接到服务器，都需要为其生成一个事件
				epoll_ctl(epollfd, EPOLL_CTL_ADD, new_fd, &ev);
            }

            //EPOLLIN表示可读
            else if (events[ix].events & EPOLLIN)
            {
            	cout << "-------------------------" << endl;
            	cout << "read..." << endl;
                //如果是已经连接的用户，并且收到数据，那么进行读入
                int sockfd = events[ix].data.fd;
                char recv_buf[1024];
                int read_len = server.recv_message(sockfd, recv_buf, 1024);
                
                result_vec.clear();
                search.search_result(recv_buf, result_vec, segment);
                if (read_len == 0)
                {
                    close(sockfd);
                    events[ix].data.fd = -1;
                }

                ev.data.fd = sockfd;
                ev.events = EPOLLOUT | EPOLLET;
                epoll_ctl(epollfd, EPOLL_CTL_MOD, sockfd, &ev);
            }
            else if (events[ix].events & EPOLLOUT) // 如果有数据发送
            {
                int sockfd = events[ix].data.fd;
              	Task task;
              	task._client_fd = sockfd;
                task._send_vec = result_vec;
                cout << "--------------" << endl;
                cout << "_send_vec.size():" << task._send_vec.size() << endl;
              	task.excute_task();
                task._send_vec.clear();
                result_vec.clear();
                ev.data.fd = sockfd;
                //写完后，这个sockfd准备读
                ev.events = EPOLLIN | EPOLLET;
                epoll_ctl(epollfd, EPOLL_CTL_MOD, sockfd, &ev);
            }
        }
    }
    close(epollfd);
    return 0;
}


