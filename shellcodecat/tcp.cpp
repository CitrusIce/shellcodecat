#include "stdafx.h"
#include <winsock2.h>
#include <windows.h>
#include <vector>
#pragma comment(lib,"ws2_32.lib")
#include <STDIO.H>
char* SocketReceive(const char* ip, int port,size_t &dataSize)
{
	WORD sockVersion = MAKEWORD(2, 2);
	WSADATA data;
	if (WSAStartup(sockVersion, &data) != 0)
	{
		return 0;
	}

	SOCKET sclient = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (sclient == INVALID_SOCKET)
	{
		printf("invalid socket !");
		return 0;
	}

	struct sockaddr_in serAddr;
	serAddr.sin_family = AF_INET;
	serAddr.sin_port = htons(port);
	serAddr.sin_addr.S_un.S_addr = inet_addr(ip);
	if (connect(sclient, (struct sockaddr *)&serAddr, sizeof(serAddr)) == SOCKET_ERROR)
	{
		printf("connect error !");
		closesocket(sclient);
		return 0;
	}
	dataSize = 0;
	std::vector<BYTE> recData;
	char buffer[2048];
	char* pData = buffer;
	int ret=0;
	do {

		ret = recv(sclient, pData, 2048, 0);
		for (int i = 0; i < ret; i++)
			recData.push_back(buffer[i]);
	} while (ret > 0);
	char* bytesData = new char[recData.size()];
	for (int i = 0; i < recData.size(); i++)
		bytesData[i] = recData[i];
	dataSize = recData.size();
	closesocket(sclient);
	WSACleanup();
	return bytesData;
}