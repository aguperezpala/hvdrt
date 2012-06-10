/*
 * Locker.h
 *
 *  Created on: 24/10/2010
 *      Author: agustin
 */

#ifndef SCOPELOCKER_H_
#define SCOPELOCKER_H_

#include <pthread.h>

class ScopeLocker{
public:
	ScopeLocker(pthread_mutex_t &Mutex) : mutex(Mutex)
	{
		pthread_mutex_lock(&mutex);
	};

	~ScopeLocker(){pthread_mutex_unlock(&mutex);};

private:
	pthread_mutex_t &mutex;

};

#endif /* SCOPELOCKER_H_ */
