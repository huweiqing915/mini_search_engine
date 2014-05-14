/*************************************************************************
	> File Name: test_socket.cpp
	> Author: huwq
	> Mail:huweiqing915@gmail.com 
	> Created Time: 2014年05月14日 星期三 16时49分11秒
 ************************************************************************/

#include "TCPSocket.h"
#include "Config.h"

using namespace std;

int main()
{
	cout << "start tcp " << endl;
	string ip, port;
	Config *p_config = Config::get_instance();
	p_config->get_file_name("server_ip", ip);
	p_config->get_file_name("server_port", port);
	TCPSocket server(ip, port);
	server.tcp_server_init();
	int new_fd;
	char recv_buf[1024];
	char send_buf[1024] = "hello client";
	while(new_fd = server.tcp_accept())
	{
		bzero(recv_buf, 1024);
		int iret = server.recv_message(new_fd, recv_buf, 1024);
		recv_buf[iret] = '\0';
		server.send_message(new_fd, send_buf, strlen(send_buf));
	}
	return 0;
}
