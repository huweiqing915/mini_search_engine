<?php 
	$host = "127.0.0.1";
	$port = "6688";
	$timeout = 15;	//set time out 
	$client_fd = socket_create (AF_INET, SOCK_STREAM, SOL_TCP);
	if(!$client_fd)
	{
		die("Unable to create client socket!\n");
	}

	// $iret = socket_set_nonblock($client_fd)
	// if(!$iret)
	// {
	// 	die("Unable to set nonblock on socket!\n");
	// }

	$time = time();

	while(!@socket_connect($client_fd, $host, $port))
	{
		$err = socket_last_error($client_fd);
		if($err == 114 || $err == 115)
		{
			if((time() - $time) >= $timeout)
			{
				socket_close($client_fd);
				die("Connect time out!\n");
			}
			sleep(1);
			continue;
		}
		die(socket_strerror($err) . "\n");
	}

	$send_buf = $_REQUEST["search_content"];
	$len = strlen($send_buf);
	$sent_bytes = socket_send($client_fd, $send_buf, $len, 0);
	if($sent_bytes == -1)
	{
		die("Send to server error");
	}

	$recv_buf = "";
	$recv_bytes = socket_recv($client_fd, $recv_buf, 65535, 0);
	if ($recv_bytes == -1) 
	{
		die("Receive from server error");
	}

	echo trim($recv_buf)."\n";//去掉接受到的字符串的首尾空格，返回给post请求的data

//	echo "Received $recv_buf from remote address $from and remote port $port1<br\>"
	socket_close($client_fd);
?>

