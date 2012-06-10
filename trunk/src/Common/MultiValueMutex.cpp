/*
 * MultiValueMutex.cpp
 *
 *  Created on: 25/03/2011
 *      Author: agustin
 */

#include "MultiValueMutex.h"


MultiValueMutex::MultiValueMutex()
{
	pthread_mutex_init(&mCounterMutex, 0);
	pthread_mutex_init(&mMutex, 0);

	// we lock the mutex
	pthread_mutex_lock(&mMutex);
	mCounter = 0;
}

MultiValueMutex::~MultiValueMutex()
{
	pthread_mutex_unlock(&mMutex);
	pthread_mutex_unlock(&mCounterMutex);
}


/* Locks only if the value of the counter is == 0, else, decrements counter */
void MultiValueMutex::lock()
{
	pthread_mutex_lock(&mCounterMutex);
	if(mCounter > 0){
		mCounter--;
		pthread_mutex_unlock(&mCounterMutex);
		return;
	}

	mCounter--;
	// if we are here then we have to really lock the other one
	pthread_mutex_unlock(&mCounterMutex);
	pthread_mutex_lock(&mMutex);
}

/* Unlocks == inc counter. */
void MultiValueMutex::unlock()
{
	ScopeLocker locker(mCounterMutex);
	mCounter++;
	if(mCounter == 1){
		pthread_mutex_unlock(&mMutex);
	}
}

