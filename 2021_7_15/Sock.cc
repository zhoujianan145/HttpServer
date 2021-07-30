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
		//��ֹ����ʼ������\n
		char c = 'x';
		while (c != '\n')
		{
			ssize_t s = recv(sock, &c, 1, 0);
			if (s > 0)
			{
				//�������\r
				if (c == '\r')
				{
					//��̽����������ַ���û�У��еĻ���ɶ
					ssize_t ss = recv(sock, &c, 1, MSG_PEEK);

					//\r\n
					if (ss > 0 && c == '\n')
					{
						//1.��\n�Ǿ��ڶ�һ��
						recv(sock, &c, 1, 0);
					}
					else// \r 
					{
						c = '\n';
					}
				}
			}

			//1.���������ַ�
			//2.�ָ�������\n
			//3.\r\n����������\r,\rת������\n
			line.push_back(c);

		}
	}
};

