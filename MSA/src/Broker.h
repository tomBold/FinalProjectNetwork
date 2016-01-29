/*
 * Broker.h
 *
 *  Created on: Dec 18, 2015
 *      Author: Tom Boldan and Gal Schlezinger
 */

#ifndef BROKER_H_
#define BROKER_H_

#include "MThread.h"
#include "TCPSocket.h"
#include "MultipleTCPSocketsListener.h"
#include "TCPMessengerServer.h"
#include "TCPMessengerProtocol.h"
#include "TCPMessengerDispatcher.h"

class Broker: public MThread
{
	TCPMessengerDispatcher* dispatcher;
	MultipleTCPSocketsListener* multiSocketListener;
	bool areBothPeersConnected;
public:
	TCPSocket* firstSocket;
	TCPSocket* secondSocket;

	Broker();
	Broker(TCPSocket* initiativeSocket, TCPSocket* recvSocket, TCPMessengerDispatcher* dispatcher);
	virtual ~Broker();

	/**
	 * Run the messages between peers
	 */
	void run();
};

#endif /* BROKER_H_ */
