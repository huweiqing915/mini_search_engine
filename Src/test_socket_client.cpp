/*************************************************************************
	> File Name: test_socket_client.cpp
	> Author: huwq
	> Mail:huweiqing915@gmail.com 
	> Created Time: 2014年05月14日 星期三 17时06分46秒
 ************************************************************************/

#include "TCPSocket.h"

using namespace std;

int main()
{
	string ip, port;
	Config *p_config = Config::get_instance();
	p_config->get_file_name("server_ip", ip);
	p_config->get_file_name("server_port", port);
	TCPSocket client(ip, port);
	client.tcp_connect();

	char send_buf[1024] = "hello server";
	client.send_message(send_buf, strlen(send_buf));

	bzero(send_buf, 1024);
	int iret = client.recv_message(send_buf, 1024);
	send_buf[iret] = '\0';
	return 0;
}
