/*
 * MThread.h
 *
 * Represents a thread handling class for our applications.
 * For using a thread, we just need to extend MThread, implement run().
 * and calling "start()".
 *
 *  Created on: Jan 30, 2016
 *      Author: Tom Boldan & Gal Schlezinger
 */

#ifndef MTHREAD_H_
#define MTHREAD_H_

#include <pthread.h>
#include <iostream>
#include <stdio.h>

class MThread {
public:
	pthread_t threadId;

public:
	/*
	 * Method to run on a different thread.
	 */
	virtual void run() = 0;

	/*
	 * Start the thread
	 */
	void start();

	/**
	 * Hold current thread until this thread is finished
	 */
	void waitForThread();
	virtual ~MThread();
};

#endif /* MTHREAD_H_ */
