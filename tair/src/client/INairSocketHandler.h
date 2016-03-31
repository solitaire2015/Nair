/*
 * INairSocketHandler.h
 *
 *  Created on: 2015��3��29��
 *      Author: paul
 */

#ifndef INAIRSOCKETHANDLER_H_
#define INAIRSOCKETHANDLER_H_

#include "NairProtocol.h"

struct INairSocketHandler
{
	virtual void OnSocketRecvData(int remotesocket, struct NairNetPacket* packet) = 0;

	virtual void OnSocketConnect(int remotesocket, unsigned int remoteip, unsigned short remoteport) = 0;

	virtual void OnSocketDisConnect(int remotesocket, unsigned int remoteip, unsigned short remoteport) = 0;
};

#endif /* INAIRSOCKETHANDLER_H_ */
