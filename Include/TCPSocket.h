/*************************************************************************
	> File Name: TCPSocket.h
	> Author: huwq
	> Mail:huweiqing915@gmail.com 
	> Created Time: 2014年05月14日 星期三 16时40分30秒
 ************************************************************************/

#ifndef TCP_SOCKET_H
#define TCP_SOCKET_H

#include <iostream>
#include <string>
#include <stdexcept>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#include "Config.h"
#include "Log.h"

#define MAX_CONNECT_NUM 5 //服务器允许连接的最大数

class TCPSocket {
public:
	TCPSocket(const std::string &ip, const std::string &port);
	~TCPSocket();

	void tcp_server_init();//初始化server_addr
	int tcp_accept();	//服务器接受客户端请求
	void tcp_connect(); //客户端请求连接服务器

	int send_message(void *buf, size_t len);	//For client
	int send_message(int fd, void *buf, size_t len);	//For server
	int recv_message(void *buf, size_t len);	//For server
	int recv_message(int fd, void *buf, size_t len);	//For client
private:
	int _sock_fd;	//socket返回的描述符，用来访问该socket;
	struct sockaddr_in _server_addr;
};

#endif


