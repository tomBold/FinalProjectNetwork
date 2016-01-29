/*
 * Guard.h
 *
 *  Created on: Feb 13, 2013
 *      Author: efi
 */

#ifndef GUARD_H_
#define GUARD_H_

#include <pthread.h>
/**
 * To init a mutex use:	pthread_mutex_t	mutex = PTHREAD_MUTEX_INITIALIZER;
 */
class Guard{
	pthread_mutex_t* mutex;
public:
	Guard(pthread_mutex_t* m){
		mutex = m;
		pthread_mutex_lock(mutex);
	}
	~Guard(){
		pthread_mutex_unlock(mutex);
	}
};

#endif /* GUARD_H_ */
