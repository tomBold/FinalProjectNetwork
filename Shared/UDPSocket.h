/*
 * UDPSocket.h
 *
 *  Created on: Jan 30, 2016
 *      Author: Tom Boldan & Gal Schlezinger
 */

#ifndef UDPSOCKET_H_
#define UDPSOCKET_H_

#include <netinet/in.h>
#include <inttypes.h>
#include <strings.h>
#include <stdio.h>
#include <iostream>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <sys/socket.h>
#include <unistd.h>

using namespace std;

class UDPSocket{
	struct sockaddr_in  s_in;
	struct sockaddr_in from;
	unsigned int fsize;
	int socket_fd;

public:
	UDPSocket(int port = 9999);

	/**
	 * Try to read
	 */
	int recv(char* buffer, int length);

	/**
	 * Send a message
	 */
	int sendTo(string msg, string ip, int port);

	/**
	 * Replay a message to the last sender
	 */
	int reply(string msg);

	/**
	 * Close
	 */
	void cclose();

	/**
	 * Gets the from address
	 */
	string fromAddr();

	/**
	 * Gets the port
	 */
	int getPort();

	/**
	 * Get the destination io and port
	 */
	string destIpAndPort();
};

#endif /* UDPSOCKET_H_ */
