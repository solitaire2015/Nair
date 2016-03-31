/*
 * NairServiceCore.h
 *
 *  Created on: 2015Äê3ÔÂ31ÈÕ
 *      Author: paul
 */

#ifndef NAIRSERVICECORE_H_
#define NAIRSERVICECORE_H_

#include "NairTypedef.h"
#include "NairLocker.h"
#include "NairProtocol.h"
#include "NairServerSocket.h"
#include "INairSocketHandler.h"
#include <vector>
#include <algorithm>

using namespace std;

class CFindRemoteSocket
{
public:

	CFindRemoteSocket(int remotesocket)
		: _remotesocket(remotesocket)
	{
	}

	bool operator() (vector<int>::value_type& value)
	{
		if(_remotesocket == value)
			return true;
		return false;
	}

private:
	int _remotesocket;
};

typedef vector<int> VECRemotes;

struct INairServerCoreHandler
{
	virtual void OnNairRemoteJoined(int remotesocket, int remotecount) = 0;

	virtual void OnNairRemoteUnJoined(int remotesocket, int remotecount) = 0;

	virtual void OnNairPutRequest(unsigned int area, unsigned int expired, unsigned int version, char* key, char* value, unsigned short& code) = 0;

	virtual void OnNairGetRequest(unsigned int area, char* key, unsigned int& version, char*& value, unsigned short& code) = 0;

	virtual void OnNairGetsRequest(unsigned int area, vector<NairKey*>& keyvalues, unsigned short& code) = 0;

	virtual void OnNairRemoveRequest(unsigned int area, char* key, unsigned short& code) = 0;

	virtual void OnNairIncrRequest(unsigned int area, unsigned int expired, unsigned int value, unsigned int defaultvalue, unsigned int& returnvalue, char* key, unsigned short& code) = 0;
};

class NairServiceCore : public INairSocketHandler
{
public:

	NairServiceCore(INairServerCoreHandler* handler);

	~NairServiceCore();

	int Start(unsigned int uport);

	void Close();

	void SendPacket(int remotesocket, unsigned int packetmsg, unsigned int packetid, void* packetdata, unsigned int packetsize);

private:

	void DisposeServerInfoRequet(int remotesocket, NairPacketHeader* header, NairServerInfoRequest* request);

	void DisposeServerJoinRequest(int remotesocket, NairPacketHeader* header, NairServerJoinRequest* request);

	void DisposePutRequest(int remotesocket, NairPacketHeader* header, NairPutRequest* request);

	void DisposeGetRequest(int remotesocket, NairPacketHeader* header, NairGetRequest* request);

	void DisposeGetsRequest(int remotesocket, NairPacketHeader* header, NairGetsRequest* request);

	void DisposeRemoveRequest(int remotesocket, NairPacketHeader* header, NairRemoveRequest* request);

	void DisposeIncrRequest(int remotesocket, NairPacketHeader* header, NairIncrRequest* request);

	void SendPutResponse(int remotesocket, unsigned int packetid, unsigned short code, unsigned int area, const char* key);

	void SendGetResponse(int remotesocket, unsigned int packetid, unsigned short code, unsigned int area, unsigned int version, const char* key, const char* value);

	void SendGetsResponse(int remotesocket, unsigned int packetid, unsigned short code, unsigned int area, vector<NairKey*> keyvalues);

	void SendRemoveResponse(int remotesocket, unsigned int packetid, unsigned short code, unsigned int area, const char* key);

	void SendIncrResponse(int remotesocket, unsigned int packetid, unsigned short code, unsigned int area, unsigned int value, const char* key);

private:

	void OnSocketRecvData(int remotesocket, NairNetPacket* packet);

	void OnSocketConnect(int remotesocket, unsigned int remoteip, unsigned short remoteport);

	void OnSocketDisConnect(int remotesocket, unsigned int remoteip, unsigned short remoteport);

private:

	NairServerSocket* _socket;

	VECRemotes _remotes;

	INairServerCoreHandler* _handler;

	NairMutex _mutex;
};

#endif /* NAIRSERVICECORE_H_ */
