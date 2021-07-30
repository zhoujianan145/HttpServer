#pragma once
#include"Sock.hpp"
#include"Protocol.hpp"
#include<pthread.h>
class HttpServer{
private:
	int port;
	int lsock;
public:
	HttpServer(int _p = 8081)
		:port(_p)
		, lsock(-1)
	{}
	void InitServer()
	{
		lsock = Sock::Socket();
		Sock::SetSockOpt(lsock);
		Sock::Bind(lsock, port);
		Sock::Listen(lsock);
	}
	void Start()
	{
		//��ʼѭ�������׽���
		for (;;)
		{

			int sock = Sock::Accept(lsock);
			if (sock < 0)
			{
				continue;
			}
			LOG(Notice, "get a new link");
			//���������滻���̳߳�
			pthread_t tid;
			int *sockp = new int(sock);
			pthread_create(&tid, nullptr, Entry::HandlerHttp, sockp);
			//�����̷߳��룬�����߳���Զ���˳�����������
			pthread_detach(tid);

		}
	}
	~HttpServer()
	{
		if (lsock >= 0)
		{
			close(lsock);
		}
	}
};
