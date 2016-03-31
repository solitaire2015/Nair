/*
 * NairProtocol.h
 *
 *  Created on: 2015Äê3ÔÂ29ÈÕ
 *      Author: paul
 */

#ifndef NAIRPROTOCOL_H_
#define NAIRPROTOCOL_H_

#define NAIR_TOKEN			 0x0010
#define NAIR_SUCCESSED		 0x00
#define NAIR_FAILED			 0x01

#define NAIR_SERVERINFO_REQUEST  0x10
#define NAIR_SERVERINFO_RESPONSE 0x11
#define NAIR_SERVERJOIN_REQUEST	 0x12
#define NAIR_SERVERJOIN_RESPONSE 0x13

#define NAIR_PUT_REQUEST	  0x01
#define NAIR_PUT_RESPONSE	  0x02
#define NAIR_GET_REQUEST      0x03
#define NAIR_GET_RESPONSE     0x04
#define NAIR_REMOVE_REQUEST   0x05
#define NAIR_REMOVE_RESPONSE  0x06
#define NAIR_GETS_REQUEST     0x07
#define NAIR_GETS_RESPONSE    0x08
#define NAIR_REMOVES_REQUEST  0x09
#define NAIR_REMOVES_RESPONSE 0xA0
#define NAIR_INCR_REQUEST     0xA1
#define NAIR_INCR_RESPONSE    0xA2

#define PACK_HEADER_SIZE sizeof(NairPacketHeader)

#pragma pack(push,1)

/*
 * Nair Packet Header
 *
 * */
struct NairPacketHeader
{
	NairPacketHeader()
	{
		PacketMsg = 0;
		PacketId = 0;
		PacketSize = 0;
	}

	NairPacketHeader(unsigned int upacketmsg, unsigned int upacketid, unsigned int upacketsize)
	{
		PacketMsg = upacketmsg;
		PacketId = upacketid;
		PacketSize = upacketsize;
	}

	unsigned int PacketMsg;
	unsigned int PacketId;
	unsigned int PacketSize;
} ;

/*
 * Nair Packet Body
 *
 * */
struct NairNetPacket
{
	NairNetPacket()
	{
		PacketHeader.PacketMsg = 0x00;
		PacketHeader.PacketSize = 0;
		PacketData = 0;
	}

	NairNetPacket(unsigned int upacketmsg, void* upacketdata, unsigned int upacketsize)
	{
		PacketHeader.PacketMsg = upacketmsg;
		PacketHeader.PacketSize = upacketsize;
		PacketData = upacketdata;
	}

	NairPacketHeader PacketHeader;
    void* PacketData;
} ;

/*
 * Nair ServerInfo Request
 *
 * */
struct NairServerInfoRequest
{
	NairServerInfoRequest()
	{
		_clientid = 0;
	}

	NairServerInfoRequest(unsigned int uclientid)
	{
		_clientid = uclientid;
	}

	unsigned int _clientid;
};

/*
 * Nair ServerInfo Response
 *
 * */
struct NairServerInfoResponse
{
	NairServerInfoResponse()
	{
		_clientid = 0;
		_clientcount = 0;
	}

	NairServerInfoResponse(unsigned int uclientid, unsigned int uclientcount)
	{
		_clientid = uclientid;
		_clientcount = uclientcount;
	}

	unsigned int _clientid;
	unsigned int _clientcount;
};

/*
 * Nair ServerJoin Request
 *
 * */
struct NairServerJoinRequest
{
	NairServerJoinRequest()
	{
		_clientid = 0;
	}

	NairServerJoinRequest(unsigned int uclientid)
	{
		_clientid = uclientid;
	}

	unsigned int _clientid;
};

/*
 * Nair ServerJoin Response
 *
 * */
struct NairServerJoinResponse
{
	NairServerJoinResponse()
	{
		_code = 0;
		_clientid = 0;
	}

	NairServerJoinResponse(unsigned short ucode, unsigned int uclientid)
	{
		_code = ucode;
		_clientid = uclientid;
	}

	unsigned short _code;
	unsigned int _clientid;
};

/*
 * Nair Put Request
 *
 * */
struct NairPutRequest
{
	NairPutRequest()
	{
		_area = 0;
		_expired = 0;
		_version = 0;
		_keysize = 0;
		_valuesize = 0;
	}

	NairPutRequest(unsigned int uarea, unsigned int uexpired, unsigned int uversion,
			unsigned int ukeysize, int uvaluesize)
	{
		_area = uarea;
		_expired = uexpired;
		_version = uversion;
		_keysize = ukeysize;
		_valuesize = uvaluesize;
	}

	unsigned int _area;
	unsigned int _expired;
	unsigned int _version;
	unsigned int _keysize;
	unsigned int _valuesize;
};

/*
 * Nair Put Response
 *
 * */
struct NairPutResponse
{
	NairPutResponse()
	{
		_code = NAIR_FAILED;
		_area = 0;
		_keysize = 0;
	}

	NairPutResponse(unsigned short ucode, unsigned int uarea,
			unsigned int ukeysize)
	{
		_code = ucode;
		_area = uarea;
		_keysize = ukeysize;
	}

	unsigned short _code;
	unsigned int _area;
	unsigned int _keysize;
};

/*
 * Nair Get Request
 *
 * */
struct NairGetRequest
{
	NairGetRequest()
	{
		_area = 0;
		_keysize = 0;
	}

	NairGetRequest(unsigned int uarea,	unsigned int ukeysize)
	{
		_area = uarea;
		_keysize = ukeysize;
	}

	unsigned int _area;
	unsigned int _keysize;
};

/*
 * Nair Get Response
 *
 * */
struct NairGetResponse
{
	NairGetResponse()
	{
		_code = NAIR_FAILED;
		_area = 0;
		_version = 0;
		_keysize = 0;
		_valuesize = 0;
	}

	NairGetResponse(unsigned short ucode, unsigned int uarea, unsigned int uversion,
			unsigned int ukeysize, unsigned int uvaluesize)
	{
		_code = ucode;
		_area = uarea;
		_version = uversion;
		_keysize = ukeysize;
		_valuesize = uvaluesize;
	}

	unsigned short _code;
	unsigned int _area;
	unsigned int _version;
	unsigned int _keysize;
	unsigned int _valuesize;
};

/*
 * Nair Remove Request
 *
 * */
struct NairRemoveRequest
{
	NairRemoveRequest()
	{
		_area = 0;
		_keysize = 0;
	}

	NairRemoveRequest(unsigned int uarea,	unsigned int ukeysize)
	{
		_area = uarea;
		_keysize = ukeysize;
	}

	unsigned int _area;
	unsigned int _keysize;
};

/*
 * Nair Remove Response
 *
 * */
struct NairRemoveResponse
{
	NairRemoveResponse()
	{
		_code = NAIR_FAILED;
		_area = 0;
		_keysize = 0;
	}

	NairRemoveResponse(unsigned short ucode, unsigned int uarea,
			unsigned int ukeysize)
	{
		_code = ucode;
		_area = uarea;
		_keysize = ukeysize;
	}

	unsigned short _code;
	unsigned int _area;
	unsigned int _keysize;
};

/*
 * Nair Gets Request
 *
 * NairPacketHeader + NairGetsRequest + Array(KeySize) + Array(KeyString)
 *
 * */
struct NairGetsRequest
{
	NairGetsRequest()
	{
		_area = 0;
		_keycount = 0;
	}

	NairGetsRequest(unsigned int uarea, unsigned int ukeycount)
	{
		_area = uarea;
		_keycount = ukeycount;
	}

	unsigned int _area;
	unsigned int _keycount;
};

/*
 * Nair Gets Response
 *
 * NairPacketHeader + NairGetsResponse + Array(ValueSize) + Array(ValueString)
 *
 * */
struct NairGetsResponse
{
	NairGetsResponse()
	{
		_code = NAIR_FAILED;
		_area = 0;
		_valuecount = 0;
	}

	NairGetsResponse(unsigned short ucode,
			unsigned int uarea, unsigned int uvaluecount)
	{
		_code = ucode;
		_area = uarea;
		_valuecount = uvaluecount;
	}

	unsigned short _code;
	unsigned int _area;
	unsigned int _valuecount;
};

/*
 * Nair Incr Request
 *
 * NairPacketHeader + NairIncrRequest + key
 *
 * */
struct NairIncrRequest
{
	NairIncrRequest()
	{
		_area = 0;
		_expired = 0;
		_value = 0;
		_defaultvalue = 0;
		_keysize = 0;
	}

	NairIncrRequest(unsigned int uarea, unsigned int uexpired,
			unsigned int uvalue, unsigned int udefaultvalue, unsigned int ukeysize)
	{
		_area = uarea;
		_expired = uexpired;
		_value = uvalue;
		_defaultvalue = udefaultvalue;
		_keysize = ukeysize;
	}

	unsigned int _area;
	unsigned int _expired;
	unsigned int _value;
	unsigned int _defaultvalue;
	unsigned int _keysize;
};

/*
 * Nair Incr Response
 *
 * NairPacketHeader + NairIncrResponse + key
 *
 * */
struct NairIncrResponse
{
	NairIncrResponse()
	{
		_code = NAIR_FAILED;
		_area = 0;
		_value = 0;
		_keysize = 0;
	}

	NairIncrResponse(unsigned short ucode, unsigned int uarea,
			unsigned int uvalue, unsigned int ukeysize)
	{
		_code = ucode;
		_area = uarea;
		_value = uvalue;
		_keysize = ukeysize;
	}

	unsigned short _code;
	unsigned int _area;
	unsigned int _value;
	unsigned int _keysize;
};

#pragma pack(pop)

#endif /* NAIRPROTOCOL_H_ */
