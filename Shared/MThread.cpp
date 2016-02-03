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

#include "MThread.h"

using namespace std;
/*
 * Method to run on a different thread.
 */
void* worker(void* arg) {
	MThread* threadObj = (MThread*) arg;
	threadObj->run();
	threadObj->threadId = 0;
	return NULL;
}

/*
 * Start the thread
 */
void MThread::start() {
	pthread_create(&threadId, NULL, worker, (void*) this);
}

/**
 * Hold current thread until this thread is finished
 */
void MThread::waitForThread() {
	pthread_join(threadId, NULL);
	threadId = 0;
}

MThread::~MThread() {
	if (threadId > 0) {
		// Kills the thread if exist
		pthread_cancel(threadId);
	}
}
