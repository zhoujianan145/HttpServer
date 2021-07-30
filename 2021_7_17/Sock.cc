#pragma once

#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<unistd.h>
#include<stdlib.h>
#include<string.h>
#include"Log.hpp"
#include<vector>
#include<fcntl.h>
#include<unordered_map>
#include<sys/wait.h>
#include<sys/sendfile.h>
#include<sstream>
#define BACK_LOG 5
class Sock{
public:
	static int Socket()
	{
		int sock = socket(AF_INET, SOCK_STREAM, 0);
		if (sock < 0)
		{
			LOG(Fatal, "socket cret error");
			exit(SocketErr);
		}
		return sock;
	}
	static void SetSockOpt(int sock)
	{
		int opt = 1;
		setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
	}
	static void Bind(int sock, int port)
	{
		struct sockaddr_in local;
		bzero(&local, sizeof(local));
		local.sin_family = AF_INET;
		local.sin_port = htons(port);
		local.sin_addr.s_addr = htonl(INADDR_ANY);
		if (bind(sock, (struct sockaddr*)&local, sizeof(local))<0)
		{
			LOG(Fatal, "socket bind error");
			exit(BindErr);
		}
	}
	static void Listen(int sock)
	{
		if (listen(sock, BACK_LOG) < 0)
		{
			LOG(Fatal, "socket listen Error");
			exit(ListenErr);
		}
	}
	static int Accept(int sock)
	{
		struct sockaddr_in peer;
		socklen_t len = sizeof(peer);
		int s = accept(sock, (struct sockaddr*)&peer, &len);

		if (s < 0)
		{
			LOG(Warning, "accepr error");
		}
		return s;


	}
	static void GetLine(int sock, string& line)
	{
		//防止不初始化就是\n
		char c = 'x';
		while (c != '\n')
		{
			ssize_t s = recv(sock, &c, 1, 0);
			if (s > 0)
			{
				//假如读到\r
				if (c == '\r')
				{
					//窥探，看后面的字符有没有，有的话是啥
					ssize_t ss = recv(sock, &c, 1, MSG_PEEK);

					//\r\n
					if (ss > 0 && c == '\n')
					{
						//1.是\n那就在读一次
						recv(sock, &c, 1, 0);
					}
					else// \r 
					{
						c = '\n';
					}
				}
			}

			//1.读到常规字符
			//2.分隔符就是\n
			//3.\r\n我们跳过了\r,\r转换成了\n
			line.push_back(c);

		}
	}
};

