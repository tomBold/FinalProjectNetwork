/*
 * ServerIO.h
 *
 * Handles the reading and writing to and from the server
 *
 *  Created on: Jan 30, 2016
 *      Author: Tom Boldan & Gal Schlezinger
 */

#ifndef SERVERIO_H_
#define SERVERIO_H_
#define INT_BYTE_LENGTH 4

#include <strings.h>
#include <string.h>
#include "TCPSocket.h"

class ServerIO {
public:
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
};


#endif /* SERVERIO_H_ */
