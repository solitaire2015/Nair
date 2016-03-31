/*
 * NairServiceCore.cpp
 *
 *  Created on: 2015Äê3ÔÂ31ÈÕ
 *      Author: paul
 */

#include "NairServiceCore.h"

NairServiceCore::NairServiceCore(INairServerCoreHandler* handler)
	: _handler(handler)
	, _socket(NULL)
{
	_socket = new NairServerSocket(static_cast<INairSocketHandler*>(this));
}

NairServiceCore::~NairServiceCore()
{
	_remotes.clear();
	if(_socket != NULL)
	{
		delete _socket;
		_socket = NULL;
	}
}

int NairServiceCore::Start(unsigned int uport)
{
	int ret = -1;
	if(_socket != NULL)
	{
		ret = _socket->InitSocket(uport);
		if(ret > 0)
			ret = _socket->AcceptSocket();
	}
	return ret;
}

void NairServiceCore::Close()
{
	_remotes.clear();
	if(_socket != NULL)
		_socket->CloseSocket();
}

void NairServiceCore::SendPacket(int remotesocket, unsigned int packetmsg, unsigned int packetid, void* packetdata, unsigned int packetsize)
{
	if(_socket != NULL)
	{
		NairNetPacket packet;
		packet.PacketData = packetdata;
		packet.PacketHeader.PacketMsg = packetmsg;
		packet.PacketHeader.PacketId = packetid;
		packet.PacketHeader.PacketSize = packetsize;
		_socket->SocketSendPacket(remotesocket, &packet);
	}
}

void NairServiceCore::DisposeServerInfoRequet(int remotesocket, NairPacketHeader* header, NairServerInfoRequest* request)
{
	//NairLock Lock(_mutex);
	NairServerInfoResponse response(request->_clientid, _remotes.size());
	SendPacket(remotesocket, NAIR_SERVERINFO_RESPONSE, header->PacketId, &response, sizeof(response));
}

void NairServiceCore::DisposeServerJoinRequest(int remotesocket, NairPacketHeader* header, NairServerJoinRequest* request)
{
	//NairLock Lock(_mutex);
	VECRemotes::iterator it_remote = find_if(_remotes.begin(), _remotes.end(), CFindRemoteSocket(remotesocket));
	if(it_remote == _remotes.end())
	{
		_remotes.push_back(remotesocket);
		if(_handler != NULL)
			_handler->OnNairRemoteJoined(remotesocket, _remotes.size());
	}
}

void NairServiceCore::DisposePutRequest(int remotesocket, NairPacketHeader* header, NairPutRequest* request)
{
    //NairLock Lock(_mutex);
	if(_handler != NULL)
	{
		unsigned short code = NAIR_FAILED;
		char* key = new char[request->_keysize+1]; 
		memset(key, 0, request->_keysize+1);
		memcpy(key, (char*)request + sizeof(NairPutRequest), request->_keysize);
		char* value = new char[request->_valuesize+1];
		memset(value, 0, request->_valuesize+1);
		memcpy(value, (char*)request + sizeof(NairPutRequest) + request->_keysize, request->_valuesize);
		_handler->OnNairPutRequest(request->_area, request->_expired, request->_version, key, value, code);
		SendPutResponse(remotesocket, header->PacketId, code, request->_area, key);	
		delete[] value;
		delete[] key;
	} 
}

void NairServiceCore::DisposeGetRequest(int remotesocket, NairPacketHeader* header, NairGetRequest* request)
{
	//NairLock Lock(_mutex);
	if(_handler != NULL)
	{
		unsigned short code = NAIR_FAILED;
		unsigned int version = 0;
		char* value = NULL;
		char key[request->_keysize+1];
		memset(key, 0, request->_keysize+1);
		strncpy(key, (char*)request + sizeof(NairGetRequest), request->_keysize);
		_handler->OnNairGetRequest(request->_area, key, version, value, code);
		SendGetResponse(remotesocket, header->PacketId, code, request->_area, version, key, value);
		if(value != NULL)
			delete[] value;
	}
}

void NairServiceCore::DisposeGetsRequest(int remotesocket, NairPacketHeader* header, NairGetsRequest* request)
{
	//NairLock Lock(_mutex);
	if(_handler != NULL)
	{
		unsigned short code = NAIR_FAILED;
		char* pdata = (char*)request + sizeof(NairGetsRequest);
		vector<int> keyssize;
		for(int i = 0; i < (int)request->_keycount; i++)
		{
			keyssize.push_back(*(unsigned int*)pdata);
			pdata += sizeof(unsigned int);
		}
		vector<NairKey*> keyvalues;
		for(int i = 0; i < (int)request->_keycount; i++)
		{
			unsigned int keysize = keyssize[i];
			char* key = new char[keysize+1];
			memset(key, 0, keysize+1);
			strncpy(key, pdata, keysize);
			NairKey* pkeyvalue = new NairKey(key);
			keyvalues.push_back(pkeyvalue);
			pdata += keysize;
		}
		_handler->OnNairGetsRequest(request->_area, keyvalues, code);
		SendGetsResponse(remotesocket, header->PacketId, code, request->_area, keyvalues);
		for(vector<NairKey*>::iterator it = keyvalues.begin(); it != keyvalues.end(); it++)
			delete *it;
		keyvalues.clear();
	}
}

void NairServiceCore::DisposeRemoveRequest(int remotesocket, NairPacketHeader* header, NairRemoveRequest* request)
{
	//NairLock Lock(_mutex);
	if(_handler != NULL)
	{
		unsigned short code = NAIR_FAILED;
		char key[request->_keysize+1];
		memset(key, 0, request->_keysize+1);
		strncpy(key, (char*)request + sizeof(NairRemoveRequest), request->_keysize);
		_handler->OnNairRemoveRequest(request->_area, key, code);
		SendRemoveResponse(remotesocket, header->PacketId, code, request->_area, key);
	}
}

void NairServiceCore::DisposeIncrRequest(int remotesocket, NairPacketHeader* header, NairIncrRequest* request)
{
	//NairLock Lock(_mutex);
	if(_handler != NULL)
	{
		unsigned short code = NAIR_FAILED;
		unsigned int returnvalue = 0;
		char key[request->_keysize+1];
		memset(key, 0, request->_keysize+1);
		strncpy(key, (char*)request + sizeof(NairIncrRequest), request->_keysize);
		_handler->OnNairIncrRequest(request->_area, request->_expired, request->_value, request->_defaultvalue, returnvalue, key, code);
		SendIncrResponse(remotesocket, header->PacketId, code, request->_area, returnvalue, key);
	}
}


void NairServiceCore::SendPutResponse(int remotesocket, unsigned int packetid, unsigned short code, unsigned int area, const char* key)
{
	NairPutResponse response(code, area, 0);
	SendPacket(remotesocket, NAIR_PUT_RESPONSE, packetid, &response, sizeof(response));
}

void NairServiceCore::SendGetResponse(int remotesocket, unsigned int packetid, unsigned short code, unsigned int area, unsigned int version, const char* key, const char* value)
{
	int keysize = strlen(key);
	int valuesize = (value == NULL ? 0 : strlen(value));
	NairGetResponse response(code, area, version, keysize, valuesize);
	int datasize = sizeof(response) + keysize + valuesize;
	char* pdata = new char[datasize];
	memset(pdata, 0, datasize);
	memcpy(pdata, &response, sizeof(response));
	pdata += sizeof(response);
	memcpy(pdata, key, keysize);
	pdata += keysize;
	if(valuesize > 0)
	{
		memcpy(pdata, value, valuesize);
		pdata += valuesize;
	}
	pdata -= sizeof(response) + keysize + valuesize;
	SendPacket(remotesocket, NAIR_GET_RESPONSE, packetid, pdata, datasize);
	delete[] pdata;
}

void NairServiceCore::SendGetsResponse(int remotesocket, unsigned int packetid, unsigned short code, unsigned int area, vector<NairKey*> keyvalues)
{
	int valuescount = keyvalues.size();
	NairGetsResponse response(code, area, valuescount);
	int datasize = sizeof(response) + valuescount * sizeof(unsigned int);
	for(vector<NairKey*>::iterator it = keyvalues.begin(); it != keyvalues.end(); it++)
		datasize += (*it)->GetValueSize();

	char* pdata = new char[datasize];
	memset(pdata, 0, datasize);
	char* ppdata = pdata;
	memcpy(pdata, &response, sizeof(response));
	pdata += sizeof(response);

	for(vector<NairKey*>::iterator it = keyvalues.begin(); it != keyvalues.end(); it++)
	{
		unsigned int valuesize = (*it)->GetValueSize();
		memcpy(pdata, &valuesize, sizeof(unsigned int));
		pdata += sizeof(unsigned int);
	}

	for(vector<NairKey*>::iterator it = keyvalues.begin(); it != keyvalues.end(); it++)
	{
		memcpy(pdata, (*it)->GetValue(), (*it)->GetValueSize());
		pdata += (*it)->GetValueSize();
	}

	SendPacket(remotesocket, NAIR_GETS_RESPONSE, packetid, ppdata, datasize);
	delete[] ppdata;
}

void NairServiceCore::SendRemoveResponse(int remotesocket, unsigned int packetid, unsigned short code, unsigned int area, const char* key)
{
	NairRemoveResponse response(code, area, 0);
	SendPacket(remotesocket, NAIR_REMOVE_RESPONSE, packetid, &response, sizeof(response));
}

void NairServiceCore::SendIncrResponse(int remotesocket, unsigned int packetid, unsigned short code, unsigned int area, unsigned int value, const char* key)
{
    int keysize = strlen(key);
	NairIncrResponse response(code, area, value, keysize);
	int datasize = sizeof(response) + keysize;
	char* pdata = new char[datasize];
	memset(pdata, 0, datasize);
	memcpy(pdata, &response, sizeof(response));
	pdata += sizeof(response);
	memcpy(pdata, key, keysize);
	pdata -= sizeof(response);
	SendPacket(remotesocket, NAIR_INCR_RESPONSE, packetid, pdata, datasize);
	delete[] pdata;
}

void NairServiceCore::OnSocketRecvData(int remotesocket, NairNetPacket* packet)
{
	switch(packet->PacketHeader.PacketMsg)
	{
		case NAIR_SERVERINFO_REQUEST:
			DisposeServerInfoRequet(remotesocket, &(packet->PacketHeader), (NairServerInfoRequest*)packet->PacketData);
			break;
		case NAIR_SERVERJOIN_REQUEST:
			DisposeServerJoinRequest(remotesocket, &(packet->PacketHeader), (NairServerJoinRequest*)packet->PacketData);
			break;
		case NAIR_PUT_REQUEST:
			DisposePutRequest(remotesocket, &(packet->PacketHeader), (NairPutRequest*)packet->PacketData);
			break;
		case NAIR_GET_REQUEST:
			DisposeGetRequest(remotesocket, &(packet->PacketHeader), (NairGetRequest*)packet->PacketData);
			break;
		case NAIR_GETS_REQUEST:
			DisposeGetsRequest(remotesocket, &(packet->PacketHeader), (NairGetsRequest*)packet->PacketData);
			break;
		case NAIR_REMOVE_REQUEST:
			DisposeRemoveRequest(remotesocket, &(packet->PacketHeader), (NairRemoveRequest*)packet->PacketData);
			break;
		case NAIR_INCR_REQUEST:
			DisposeIncrRequest(remotesocket, &(packet->PacketHeader), (NairIncrRequest*)packet->PacketData);
			break;
	}
}

void NairServiceCore::OnSocketConnect(int remotesocket, unsigned int remoteip, unsigned short remoteport)
{
}

void NairServiceCore::OnSocketDisConnect(int remotesocket, unsigned int remoteip, unsigned short remoteport)
{
	VECRemotes::iterator it_remote = find_if(_remotes.begin(), _remotes.end(), CFindRemoteSocket(remotesocket));
	if(it_remote != _remotes.end())
	{
		_remotes.erase(it_remote);
		if(_handler != NULL)
			_handler->OnNairRemoteUnJoined(remotesocket, _remotes.size());
	}
}
