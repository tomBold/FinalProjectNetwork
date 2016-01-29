//============================================================================
// Name        : TCP Messenger Server
// Author      : Tom Boldan and Gal Schlezinger
// Version     :
// Copyright   :
// Description : TCP Messenger application
//============================================================================
#pragma once

#ifndef TCPMESSENGERSERVER_H__
#define TCPMESSENGERSERVER_H__
#define INT_BYTE_LENGTH 4

#include <strings.h>
#include <string.h>
#include <map>
#include <vector>
#include <set>
#include "MThread.h"
#include "TCPSocket.h"
#include "MultipleTCPSocketsListener.h"
#include "TCPMessengerProtocol.h"
#include "TCPMessengerDispatcher.h"
#include "AuthDispatcher.h"

class AuthDispatcher;

using namespace std;

class TCPMessengerServer : public MThread{
	TCPSocket* serverSocket;

	AuthDispatcher* authDispatcher;

public:
	/**
	 * Build the Messenger server
	 */
	TCPMessengerDispatcher* dispatcher;
	TCPMessengerServer();
	~TCPMessengerServer();

	/**
	 * print out the list of connected clients (for debug)
	 */
	void listPeers();

	/**
	 * close the server
	 */
	void close();

	/**
	 * read command from peer
	 */
	static int readCommandFromPeer(TCPSocket* peer);

	/**
	 * read data from peer
	 */
	static string readDataFromPeer(TCPSocket* peer);

	/**
	 * send command to peer
	 */
	static void sendCommandToPeer(TCPSocket* peer, int command);

	/**
	 * send data to peer
	 */
	static void sendDataToPeer(TCPSocket* peer, string msg);

	void run();

	void userLogin(TCPSocket* peer, string name);
};
#endif
