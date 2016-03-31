/*
 * NairLocker.h
 *
 *  Created on: Apr 24, 2015
 *      Author: rl5c
 */

#ifndef NAIRLOCKER_H_
#define NAIRLOCKER_H_

#include <pthread.h>

class INairLock
{
public:

	virtual ~INairLock() {}

	virtual void Lock() const = 0;

	virtual void Unlock() const = 0;
};

class NairMutex : public INairLock
{
public:

	NairMutex();

	~NairMutex();

	virtual void Lock() const;

	virtual void Unlock() const;

private:

	mutable pthread_mutex_t _mutex;
};

class NairLock
{
public:

	NairLock(const INairLock& m);

	~NairLock();

private:

	const INairLock& _lock;
};

#endif /* NAIRLOCKER_H_ */
