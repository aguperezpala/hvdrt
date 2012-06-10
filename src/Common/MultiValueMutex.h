/*
 * MultiValueMutex.h
 *
 *  Created on: 25/03/2011
 *      Author: agustin
 */

#ifndef MULTIVALUEMUTEX_H_
#define MULTIVALUEMUTEX_H_

#include <pthread.h>

#include "ScopeLocker.h"

class MultiValueMutex {
public:
	MultiValueMutex();
	~MultiValueMutex();

	/* Locks only if the value of the counter is == 0, else, decrements counter */
	void lock();

	/* Unlocks == inc counter. */
	void unlock();

private:
	// the mutex used to access the counter
	pthread_mutex_t mCounterMutex;
	// real mutex
	pthread_mutex_t mMutex;
	unsigned int mCounter;
};


#endif /* MULTIVALUEMUTEX_H_ */
