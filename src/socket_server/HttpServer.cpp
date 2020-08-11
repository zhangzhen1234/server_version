#include "HttpServer.h"
#include <iostream>
using namespace std;
bool HttpServer::Socket() 
{
	_RecvSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (_RecvSocket < 0) {
		perror("socket error");
		return false;
	}
	return true;
}
bool HttpServer::Bind(int port)
{
	RecvAddr.sin_family = AF_INET;
	RecvAddr.sin_port = htons(port);
	// RecvAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	RecvAddr.sin_addr.s_addr =inet_addr("192.168.8.100");
	int ret=bind(_RecvSocket, (struct sockaddr *)&RecvAddr, sizeof(RecvAddr));
	if (ret < 0) {
		perror("bind error");
		return false;
	}
	return true;

}
bool HttpServer::Recv(Json::Value &buf)
{
	// int SendAddrSize =  sizeof(RecvAddr);
    socklen_t  SendAddrSize=sizeof(RecvAddr);
	int ret = recvfrom(_RecvSocket, RecvBuf, BufLen, 0, (struct sockaddr*)&RecvAddr, &SendAddrSize);
	char RecvBuf_data[4096] = {0};
	strncpy(RecvBuf_data, RecvBuf + 6, ret-6);
	if (ret < 0) {
		perror("recvfrom error");
		return false;
	};
	Json::Value val;
	Json::Reader reader;
	if (reader.parse(RecvBuf_data, val))
	{
		buf = val;
	}
	return true;
}
bool HttpServer::Send(Json::Value &val)
{

	char	buf[4096];
	unsigned char    Frame_header[2] = { 0xD2,0xC1};
	char    Frame_length[2] = {};
	char    Frame_CRC[2]= { 0x00,0x00 };
    
	std::string SendBuf;
	Json::FastWriter writer;
	SendBuf = writer.write(val);
	int lenth_send = strlen(SendBuf.c_str());
	memcpy(buf, Frame_header, 2);
	memcpy(buf + 4, Frame_CRC, 2);
	memcpy(buf + 6, SendBuf.c_str(), lenth_send - 1);
	lenth_send = lenth_send + 6 - 1;
	buf[3] = (lenth_send >> 8) & 0xff;  
	buf[2] = lenth_send & 0xff;
	//int ret =sendto(_RecvSocket, SendBuf.c_str(), BufLen, 0, (struct sockaddr*)&RecvAddr, sizeof(RecvAddr));
	int ret = sendto(_RecvSocket, buf, lenth_send, 0, (struct sockaddr*)&RecvAddr, sizeof(RecvAddr));
	if (ret == -1) {
            printf("sendto failed ! error message :%s\n", strerror(errno));
            return false;
        }
	return true;
}
HttpServer::~HttpServer()
{
	close(_RecvSocket);
}