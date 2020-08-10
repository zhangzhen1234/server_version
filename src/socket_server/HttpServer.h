#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <signal.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include<json/json.h>
#include <string.h>


class HttpServer
{
public:
	
	HttpServer():_RecvSocket(-1){

	};
	~HttpServer();
	bool Socket();
	bool Bind(int port);
	bool Recv(Json::Value &buf);
	bool Send(Json::Value &value);
private:
	int _RecvSocket;	
	struct sockaddr_in RecvAddr;
	int SendAddrSize;
	char RecvBuf[1024];
	int BufLen = 4096;
};

