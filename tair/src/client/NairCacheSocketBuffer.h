/*
 * NairCacheSocketBuffer.h
 *
 *  Created on: 2015Äê3ÔÂ29ÈÕ
 *      Author: paul
 */

#ifndef NAIRCACHESOCKETBUFFER_H_
#define NAIRCACHESOCKETBUFFER_H_

#include "NairProtocol.h"
#include <string.h>

#define NAIR_BUFFER_SIZE 		640000
#define NAIR_BUFFER_EXPAND_SIZE 2

#define NAIR_BUFFER_BODY_SIZE 	(m_nOffset - sizeof(struct NairPacketHeader))

#define NAIR_HASFULL_PACKET ( \
        (sizeof(struct NairPacketHeader) <= m_nOffset) && \
        ((((NairPacketHeader*)m_pMsgBuffer)->PacketSize) <= NAIR_BUFFER_BODY_SIZE) \
)

#define NAIR_ISVALID_PACKET(nairNetPacket) \
       ((nairNetPacket.PacketHeader.PacketSize > 0) && (nairNetPacket.PacketData != NULL))

#define NAIR_FIRSTPACKET_SIZE (sizeof(struct NairPacketHeader) + ((struct NairPacketHeader*)m_pMsgBuffer)->PacketSize)

class NairCacheSocketBuffer
{
public:

	NairCacheSocketBuffer();

	~NairCacheSocketBuffer();

	int GetBufferSize() const;

	int ReBufferSize(int);

	bool IsFitPacketHeadSize() const;

	bool IsHasFullPacket() const;

	bool Append(char *pBuf, int nSize);

	const char *GetBufferContents() const;

	void Reset();

	void Poll();

private:

	char *m_pMsgBuffer;

	int m_nBufferSize;

	int m_nOffset;
};

#endif /* NAIRCACHESOCKETBUFFER_H_ */
