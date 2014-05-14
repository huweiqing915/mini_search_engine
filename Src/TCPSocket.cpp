/*************************************************************************
	> File Name: TCPSocket.cpp
	> Author: huwq
	> Mail:huweiqing915@gmail.com 
	> Created Time: 2014年05月14日 星期三 16时42分12秒
 ************************************************************************/

#include "TCPSocket.h"

using namespace std;

TCPSocket::TCPSocket(const string &ip, const string &port):_sock_fd(0) 
{
	bzero(&_server_addr, sizeof(sockaddr_in));
	_server_addr.sin_family = AF_INET;
	_server_addr.sin_port = htons(atoi(port.c_str()));
	//_server_addr.sin_addr.s_addr = inet_addr(ip.c_str()); //或INADDR_ANY
	_server_addr.sin_addr.s_addr = INADDR_ANY;
}

TCPSocket::~TCPSocket() 
{
	close(_sock_fd);
}

void TCPSocket::tcp_server_init()
{
	//create socket
	_sock_fd = socket(AF_INET, SOCK_STREAM, 0);
	if(-1 == _sock_fd)
	{
		LogFatal("Server create socket failed!");
		throw runtime_error("Server create socket failed!");
	}
	//bind
	if(bind(_sock_fd, (struct sockaddr*)&_server_addr, sizeof(_server_addr)) == -1)
	{
		close(_sock_fd);
		LogFatal("Server bind failed!");
		throw runtime_error("Server bind failede!");
	}
	//listen
	int iret = listen(_sock_fd, MAX_CONNECT_NUM);	//监听_sock_fd，并设置其允许的最大连接数 
	if(-1 == iret)
	{
		close(_sock_fd);
		LogError("Server listen error! ");
		throw runtime_error("Server listen error!");
	}
	cout << "Server Initialization Done..." << endl;
	LogInfo("Server initialization done!");
	cout << "Wait client connect..." << endl;
}

int TCPSocket::tcp_accept()	//服务器接收
{
	struct sockaddr_in client_addr;
	socklen_t len = sizeof(client_addr);
	bzero(&client_addr, len);
	//accept 等待客户端的connect，返回新的fd，客户端的sockaddr_in
	int new_fd = accept(_sock_fd, (struct sockaddr*)&client_addr, &len);
	if(-1 == new_fd)
	{
		close(_sock_fd);
		LogFatal("Server accept error!");
		throw runtime_error("Server accept error");
	}
	LogInfo("%s:%d success connect...", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));
	return new_fd;
}

//客户端请求连接服务器
void TCPSocket::tcp_connect()	
{
	_sock_fd = socket(AF_INET, SOCK_STREAM, 0);
	if(-1 == _sock_fd)
	{
		LogFatal("Client create socket failed!");
		throw runtime_error("client create socket failed!");
	}
	int iret = connect(_sock_fd, (struct sockaddr*)&_server_addr, sizeof(_server_addr));
	if(-1 == iret)
	{
		close(_sock_fd);
		LogFatal("Client connect failed!");
		throw runtime_error("Client connect failed!");
	}
}

int TCPSocket::send_message(void *buf, size_t len)
{
	int iret = send(_sock_fd, buf, len, 0);
	cout << "Recv message: " << (char*)buf << endl;
	return iret;
}
int TCPSocket::send_message(int fd, void *buf, size_t len)
{
	int iret = send(fd, buf, len, 0);
	cout << "Recv message: " << (char*)buf << endl;
	return iret;
}

int TCPSocket::recv_message(void *buf, size_t len)
{
	int iret = recv(_sock_fd, buf, len, 0);
	cout << "Send message: " << (char*)buf << endl;
	return iret;
}

int TCPSocket::recv_message(int fd, void *buf, size_t len)
{
	int iret = recv(fd, buf, len, 0);
	cout << "Send message: " << (char*)buf << endl;
	return iret;
}

