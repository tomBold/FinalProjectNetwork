/*
 * UDPSocket.cpp
 *
 * Implements a UDP Socket.
 *
 *  Created on: Jan 30, 2016
 *      Author: Tom Boldan & Gal Schlezinger
 */

#include "UDPSocket.h"

/**
 * Ctor to create a UDP socket by port
 */
UDPSocket::UDPSocket(int port) {
	/**
	 * int socket(int domain, int type, int protocol);
	 * creates a UDP socket
	 * AF_INET - IPv4 Internet protocols
	 * SOCK_DGRAM - UDP
	 * 0 - default protocol type fo UDP
	 */
	socket_fd = socket(AF_INET, SOCK_DGRAM, 0);

	// Clear the s_in struct
	bzero((char *) &s_in, sizeof(s_in)); /* They say you must do this    */

	// Sets the sin address
	s_in.sin_family = (short) AF_INET;
	s_in.sin_addr.s_addr = htonl(INADDR_ANY); /* WILDCARD */
	s_in.sin_port = htons((u_short) port);

	fsize = sizeof(from);

	// Bind the socket on the specified address
	if (bind(socket_fd, (struct sockaddr *) &s_in, sizeof(s_in)) < 0) {
		perror("Error naming channel");
	}
}

/**
 * Try to read a message in a specific length
 */
int UDPSocket::recv(char* buffer, int length) {
	return recvfrom(socket_fd, buffer, length, 0, (struct sockaddr *) &from,
			&fsize);
}

/**
 * Send a message to specific ip and port
 */
int UDPSocket::sendTo(string msg, string ip, int port) {
	// Clear the s_in struct
	struct sockaddr_in toAddr;
	bzero((char *) &toAddr, sizeof(toAddr));

	// Sets the sin address
	toAddr.sin_family = AF_INET;
	toAddr.sin_addr.s_addr = inet_addr(ip.data());
	toAddr.sin_port = htons(port);

	return sendto(socket_fd, msg.data(), msg.length(), 0,
			(struct sockaddr *) &toAddr, sizeof(toAddr));
}

/**
 * Replay a message to the last sender.
 */
int UDPSocket::reply(string msg) {
	return sendto(socket_fd, msg.data(), msg.length(), 0,
			(struct sockaddr *) &from, sizeof(from));
}

/*
 * Close the UDP socket
 */
void UDPSocket::cclose() {
	shutdown(socket_fd, SHUT_RDWR);
	close(socket_fd);
}

/**
 * Gets the sender address
 */
string UDPSocket::fromAddr() {
	return inet_ntoa(from.sin_addr);
}

/**
 * Gets the port
 */
int UDPSocket::getPort() {
	struct sockaddr_in sin;
	socklen_t len = sizeof(sin);

	if (getsockname(this->socket_fd, (struct sockaddr *) &sin, &len) == -1) {
		perror("Error in get socket name");
		return -1;
	} else {
		return ntohs(sin.sin_port);
	}
}

/**
 * Gets the destination ip and port.
 */
string UDPSocket::destIpAndPort() {
	string str = fromAddr() + ":";
	char buff[10];
	sprintf(buff, "%d", ntohs(from.sin_port));
	str.append(buff);

	return str;
}

