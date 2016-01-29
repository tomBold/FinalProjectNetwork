/*
 * AsyncTCPMessengerReader.h
 *
 *  Created on: Dec 11, 2015
 *      Author: user
 */

#ifndef ASYNCTCPMESSENGERREADER_H_
#define ASYNCTCPMESSENGERREADER_H_
#pragma once

#include "TCPSocket.h"
#include "MThread.h"
#include "TCPMessengerClient.h"

class TCPMessengerClient;

/**
 * Listens to the server socket and sends the client the messages
 */
class AsyncTCPMessengerReader : public MThread {
	TCPMessengerClient* client;
	TCPSocket* socket;

public:
	AsyncTCPMessengerReader();
	AsyncTCPMessengerReader(TCPSocket* socket, TCPMessengerClient* client);
	virtual ~AsyncTCPMessengerReader();
	void run();
};

#endif /* ASYNCTCPMESSENGERREADER_H_ */
