/*
 * NairServerSocket.h
 *
 *  Created on: 2015Äê3ÔÂ29ÈÕ
 *      Author: paul
 */

#ifndef NAIRSERVERSOCKET_H_
#define NAIRSERVERSOCKET_H_

#include "NairProtocol.h"
#include "NairCacheSocketBuffer.h"
#include "INairSocketHandler.h"
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <unistd.h>

#define NAIR_DEFAULT_PORT 8999
#define NAIR_SOCKET_READBUF_SIZE 640000

struct ComEndPoint
{
	ComEndPoint()
	{
		RemoteSocket = 0;
		SocketHandler = NULL;
	}

	ComEndPoint(int uremotesocket, sockaddr_in uremoteaddr, INairSocketHandler* psockethandle)
	{
		RemoteSocket = uremotesocket;
		RemoteAddr = uremoteaddr;
		SocketHandler = psockethandle;
	}

	int RemoteSocket;
	sockaddr_in RemoteAddr;
	NairCacheSocketBuffer CacheBuffer;
	INairSocketHandler* SocketHandler;
};

void* DoAcceptRemoteSocket(void* socketparam);

class NairServerSocket
{
public:

	NairServerSocket(INairSocketHandler* handler);

	~NairServerSocket();

	int InitSocket(unsigned short uport);

	void CloseSocket();

	int AcceptSocket();

	void SocketSendPacket(int remotesocket, NairNetPacket* packet);

private:

	int _socket;

	unsigned short _port;

	INairSocketHandler* _handler;
};

#endif /* NAIRSERVERSOCKET_H_ */
