/*
 * NairServerSocket.cpp
 *
 *  Created on: 2015��3��29��
 *      Author: paul
 */

#include "NairServerSocket.h"
#include <string.h>

NairServerSocket::NairServerSocket(INairSocketHandler* handler)
	: _handler(handler)
	, _socket(0)
	, _port(NAIR_DEFAULT_PORT)
{
}

NairServerSocket::~NairServerSocket()
{
	CloseSocket();
}

int NairServerSocket::InitSocket(unsigned short uport)
{
	if(_socket != 0)
		return -1;

	if(uport < 0 || uport > 65535)
		return -1;
	else if(uport != 0)
		_port = uport;

	if((_socket = socket(AF_INET, SOCK_STREAM, 0)) == -1)
	{
		_socket = 0;
		return -1;
	}
	int flag = 1;
	setsockopt(_socket, IPPROTO_TCP, TCP_NODELAY, (const void *)(&flag), sizeof(flag)); //设置TCP小包不缓冲发送
	setsockopt(_socket, SOL_SOCKET, SO_REUSEADDR, (const void *)(&flag), sizeof(flag)); //启用socket地址重用
	struct linger lingerTime;
	lingerTime.l_onoff = 0; //close socket 后立即关闭socket开关
    lingerTime.l_linger = 0; //close socket 后延时0秒立即系统回收
	setsockopt(_socket, SOL_SOCKET, SO_LINGER, (const void *)(&lingerTime), sizeof(lingerTime));
	int bufsize = NAIR_SOCKET_READBUF_SIZE;
	setsockopt(_socket, SOL_SOCKET, SO_SNDBUF, (const void *)(&bufsize), sizeof(bufsize));
	setsockopt(_socket, SOL_SOCKET, SO_RCVBUF, (const void *)(&bufsize), sizeof(bufsize));

	struct sockaddr_in server_addr;
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(_port);
	server_addr.sin_addr.s_addr = INADDR_ANY;
	bzero(&(server_addr.sin_zero), 8);
	if(bind(_socket, (struct sockaddr *)&server_addr, sizeof(struct sockaddr)) == -1)
	{
		CloseSocket();
		return -1;
	}

	if(listen(_socket, 0) == -1)
	{
		CloseSocket();
		return -1;
	}

	return _socket;
}

void NairServerSocket::CloseSocket()
{
	if(_socket > 0)
	{
		::close(_socket);
		_socket = 0;
	}
}

int NairServerSocket::AcceptSocket()
{
	if(_socket <= 0)
		return -1;

	while(1)
	{
		struct sockaddr_in remote_addr;
		int addr_len = sizeof(remote_addr);
		int remotesocket = accept(_socket, (struct sockaddr *)&remote_addr, (socklen_t *)&addr_len);
		if(remotesocket <= 0)
		{
			if(errno != EINTR)
				return -1;
		}
		else
		{
			pthread_t id;
			ComEndPoint* comendpoint = new ComEndPoint(remotesocket, remote_addr, _handler);
			int ret = pthread_create(&id, NULL, DoAcceptRemoteSocket, (void*)comendpoint);
			if(ret != 0)
			{
				::close(remotesocket);
				delete comendpoint;
			}
			else
			{
				if(_handler != NULL)
					_handler->OnSocketConnect(remotesocket, remote_addr.sin_addr.s_addr, remote_addr.sin_port);
			}
		}
	}
	return 0;
}

void NairServerSocket::SocketSendPacket(int remotesocket, NairNetPacket* packet)
{
	if(remotesocket > 0)
	{
		int nLen = PACK_HEADER_SIZE + packet->PacketHeader.PacketSize;
		char* pBuf = new char[nLen];
		memset((void*)pBuf, 0, nLen);
		memcpy((void*)pBuf, (void*)&packet->PacketHeader, PACK_HEADER_SIZE);
		pBuf += PACK_HEADER_SIZE;
		memcpy((void*)pBuf, (void*)packet->PacketData, packet->PacketHeader.PacketSize);
		pBuf -= PACK_HEADER_SIZE;
		::send(remotesocket, (void*)pBuf, nLen, 0);
		delete[] pBuf;
		//::write(remotesocket, (void*)&packet->PacketHeader, sizeof(packet->PacketHeader));
		//::write(remotesocket, packet->PacketData, packet->PacketHeader.PacketSize);
	}
}

void* DoAcceptRemoteSocket(void* socketparam)
{
	if(socketparam == NULL)
		return NULL;

	struct ComEndPoint* comendpoint = (struct ComEndPoint *)socketparam;
	if(comendpoint->SocketHandler == NULL)
	{
		::close(comendpoint->RemoteSocket);
		delete comendpoint;
		return NULL;
	}

	int recvsize = 0;
	char pBuf[NAIR_SOCKET_READBUF_SIZE];
	while(1)
	{
		if(!(comendpoint->CacheBuffer.IsHasFullPacket()))
		{
			memset(pBuf, 0, NAIR_SOCKET_READBUF_SIZE);
			recvsize = recv(comendpoint->RemoteSocket, pBuf, NAIR_SOCKET_READBUF_SIZE, 0);
			if(recvsize <= 0)
			{
				if(errno != EINTR)
					goto SOCKETCLOSE;
				else
					continue;
			}
			else
			{
				comendpoint->CacheBuffer.Append(pBuf, recvsize);
				continue;
			}
		}

		if(recvsize == 0 || (!(comendpoint->CacheBuffer.IsHasFullPacket())))
			goto SOCKETCLOSE;

		while(comendpoint->CacheBuffer.IsHasFullPacket())
		{
			NairNetPacket nairnetpacket;
			char* pdata = (char*)comendpoint->CacheBuffer.GetBufferContents();
			nairnetpacket.PacketHeader = *((NairPacketHeader*)pdata);
			nairnetpacket.PacketData = new char[nairnetpacket.PacketHeader.PacketSize];
			memset(nairnetpacket.PacketData, 0, nairnetpacket.PacketHeader.PacketSize);
			memcpy(nairnetpacket.PacketData, (pdata + sizeof(NairPacketHeader)), nairnetpacket.PacketHeader.PacketSize);
			comendpoint->SocketHandler->OnSocketRecvData(comendpoint->RemoteSocket, &nairnetpacket);
			delete[] ((char*)nairnetpacket.PacketData);
			comendpoint->CacheBuffer.Poll();
		}
	}

SOCKETCLOSE:
	if(comendpoint->SocketHandler != NULL)
		comendpoint->SocketHandler->OnSocketDisConnect(comendpoint->RemoteSocket, comendpoint->RemoteAddr.sin_addr.s_addr, comendpoint->RemoteAddr.sin_port);
	::close(comendpoint->RemoteSocket);
	delete comendpoint;
	return NULL;
}
