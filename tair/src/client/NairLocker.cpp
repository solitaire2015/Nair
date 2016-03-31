/*
 * NairLocker.cpp
 *
 *  Created on: Apr 24, 2015
 *      Author: rl5c
 */

#include "NairLocker.h"

NairMutex::NairMutex()
{
	pthread_mutex_init(&_mutex, NULL);
}

NairMutex::~NairMutex()
{
	pthread_mutex_destroy(&_mutex);
}

void NairMutex::Lock() const
{
	pthread_mutex_lock(&_mutex);
}

void NairMutex::Unlock() const
{
	pthread_mutex_unlock(&_mutex);
}

NairLock::NairLock(const INairLock& m)
	: _lock(m)
{
	_lock.Lock();
}

NairLock::~NairLock()
{
	_lock.Unlock();
}
