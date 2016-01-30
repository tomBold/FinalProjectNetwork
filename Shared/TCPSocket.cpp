
#include "TCPSocket.h"
using namespace std;

/**
 * Creates a TCP socket without listening
 */
TCPSocket::TCPSocket(int connected_sock, struct sockaddr_in serverAddr,
		struct sockaddr_in peerAddr) {
	this->connected_sock = connected_sock;
	this->serverAddr = serverAddr;
	this->peerAddr = serverAddr;
}

/**
 * Creates a TCP socket server
 */
TCPSocket::TCPSocket(int port) {
	// Open TCP socket
	this->connected_sock = socket(AF_INET, SOCK_STREAM, 0);
	if (this->connected_sock < 0) {
		perror("Error opening channel");
	}

	int enable = 1;
	if (setsockopt(this->connected_sock, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int)) < 0){
		perror("setsockopt(SO_REUSEADDR) failed");
	}

	// Set the server address for binding
	bzero(&this->serverAddr, sizeof(this->serverAddr));
	this->serverAddr.sin_family = AF_INET;
	this->serverAddr.sin_port = htons(port);

	// Bind the socket to the address
	if (bind(this->connected_sock, (struct sockaddr *) &this->serverAddr,
			sizeof(this->serverAddr)) < 0) {
		perror("Error naming channel");
	}
	printf("Server is alive and waiting for socket connection from client.\n");

}

/**
 * Creates a TCP Socket client
 */
TCPSocket::TCPSocket(string peerIp, int port) {
	// Open TCP socket
	printf("Client is alive and establishing socket connection.\n");
	this->connected_sock = socket(AF_INET, SOCK_STREAM, 0);
	if (this->connected_sock < 0) {
		perror("Error opening channel");
	}

	// Set the peer address to connect to
	bzero(&this->peerAddr, sizeof(this->peerAddr));
	this->peerAddr.sin_family = AF_INET;
	this->peerAddr.sin_addr.s_addr = inet_addr(peerIp.c_str());
	this->peerAddr.sin_port = htons(port);

	// Connect the socket to the peer server
	if (connect(this->connected_sock, (struct sockaddr *) &this->peerAddr,
			sizeof(this->peerAddr)) < 0) {
		perror("Error establishing communications");
	}
}

/**
 * Listens to the port and accept connections
 */
TCPSocket* TCPSocket::listenAndAccept() {
	// Listen on the socket
	listen(this->connected_sock, 1);

	// Call accept on the socket
	socklen_t len = sizeof(this->serverAddr);
	int connect_sock = accept(this->connected_sock,
			(struct sockaddr *) &this->serverAddr, &len);

	TCPSocket* tcpSocket = new TCPSocket(connect_sock, this->serverAddr,
			this->peerAddr);

	// Return new TCPSocket object holding the new secondary server socket returned from the accept call
	return tcpSocket;
}

/**
 * Receive a message
 */
int TCPSocket::recv(char* buffer, int length) {
	int n = read(this->connected_sock, buffer, length);

	if (n < 0) {
		perror("recvfrom");
		return -1;
	}

	return n;
}

/**
 * Sends a message
 */
int TCPSocket::send(const char* msg, int len) {
	string s = msg;
	// Call send to and sent the message to the destination address
	int n = write(this->connected_sock, msg, len);
	if (n < 0) {
		perror("Sendto");
		return -1;
	}

	return n;
}

/**
 * Shutdown and close the socket
 */
void TCPSocket::cclose() {
	shutdown(this->connected_sock, SHUT_RDWR);
	close(this->connected_sock);
}
/**
 * Return the session destination peer address
 */
string TCPSocket::fromAddr() {
	return inet_ntoa(this->peerAddr.sin_addr);
}

string TCPSocket::destIpAndPort()
{
	string str = fromAddr() + ":";
	char buff[10];
	sprintf(buff, "%d", ntohs(peerAddr.sin_port));
	str.append(buff);

	return str;
}
