/*
 * NairCacheSocketBuffer.cpp
 *
 *  Created on: 2015Äê3ÔÂ29ÈÕ
 *      Author: paul
 */

#include "NairCacheSocketBuffer.h"

NairCacheSocketBuffer::NairCacheSocketBuffer()
	: m_pMsgBuffer(NULL)
	, m_nBufferSize(NAIR_BUFFER_SIZE)
	, m_nOffset(0)
{
	m_pMsgBuffer = new char[NAIR_BUFFER_SIZE];
	memset(m_pMsgBuffer, 0, NAIR_BUFFER_SIZE);
}

NairCacheSocketBuffer::~NairCacheSocketBuffer()
{
	if(m_pMsgBuffer != NULL)
	{
		delete [] m_pMsgBuffer;
		m_pMsgBuffer = NULL;
	}
	m_nBufferSize = 0;
	m_nOffset = 0;
}

int NairCacheSocketBuffer::GetBufferSize() const
{
	return this->m_nOffset;
}

int NairCacheSocketBuffer::ReBufferSize(int nAppendSize)
{
	char* oBuffer = NULL;
	if(m_pMsgBuffer != NULL)
	{
		oBuffer = m_pMsgBuffer;
		m_pMsgBuffer = NULL;
	}

	if(oBuffer != NULL)
	{
		try
		{
			nAppendSize = (nAppendSize < 64 ? 64 : nAppendSize);
			m_nBufferSize = NAIR_BUFFER_EXPAND_SIZE * nAppendSize + m_nBufferSize;
			m_pMsgBuffer = new char[m_nBufferSize];
			memset(m_pMsgBuffer, 0, m_nBufferSize);
			memcpy(m_pMsgBuffer, oBuffer, m_nOffset);
		}
		catch(...)
		{
			throw;
		}
		delete[] oBuffer;
	}
	return m_nBufferSize;
}

bool NairCacheSocketBuffer::IsFitPacketHeadSize() const
{
	return sizeof(NairPacketHeader) <= m_nOffset;
}

bool NairCacheSocketBuffer::IsHasFullPacket() const
{
	if(!IsFitPacketHeadSize())
		return false;

	return NAIR_HASFULL_PACKET;
}

bool NairCacheSocketBuffer::Append(char *pBuf, int nSize)
{
	try
	{
		if(m_nOffset + nSize > m_nBufferSize)
			ReBufferSize(nSize);
		memcpy(m_pMsgBuffer + sizeof(char) * m_nOffset, pBuf, nSize);
		m_nOffset += nSize;
	}
	catch(...)
	{
		return false;
	}
	return true;
}

const char * NairCacheSocketBuffer::GetBufferContents() const
{
	return m_pMsgBuffer;
}

void NairCacheSocketBuffer::Reset()
{
	if(m_nOffset > 0)
	{
		m_nOffset = 0;
	    memset(m_pMsgBuffer, 0, m_nBufferSize);
	}
}

void NairCacheSocketBuffer::Poll()
{
	if(m_nOffset == 0 || m_pMsgBuffer == NULL)
		return;

	if(IsFitPacketHeadSize() && NAIR_HASFULL_PACKET)
	{
		int firstpacketsize = NAIR_FIRSTPACKET_SIZE;
		memcpy(m_pMsgBuffer, m_pMsgBuffer + firstpacketsize * sizeof(char), m_nOffset - firstpacketsize);
		m_nOffset -= firstpacketsize;
		if(m_nOffset == 0)
			memset(m_pMsgBuffer, 0, m_nBufferSize);
	}
}





