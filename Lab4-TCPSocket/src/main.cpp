//============================================================================
// Name        : TCP socket test
// Author      : Eliav Menachi
// Version     :
// Copyright   : Your copyright notice
// Description :
//============================================================================

#include <iostream>
#include <string.h>
#include "TCPSocket.h"
#include "MThread.h"
#include <stdlib.h>

using namespace std;

string message;

class TCPServer: public MThread {
	TCPSocket* serverSock;

public:
	TCPServer(int port) {
		serverSock = new TCPSocket(port);
	}

	void run() {
		TCPSocket* secondarySock = serverSock->listenAndAccept();
		if (secondarySock != NULL) {
			char buffer[100];
			memset((void*) buffer, 0, 100);
			int msgLen;
			secondarySock->recv((char*) &msgLen, 4);
			msgLen = ntohl(msgLen);
			int rc = secondarySock->recv(buffer, msgLen);
			cout << "recv msg:" << buffer << endl;
			if (rc != (int) message.length()) {
				perror("FAIL1: receive different message length");
			}
			if (message != buffer) {
				perror("FAIL2: receive different message");
			}

			message = "Test2 2222222222222222222222222222222";
			cout << "sending message:" << message << endl;
			msgLen = htonl(message.length());
			secondarySock->send((char*) &msgLen, 4);
			secondarySock->send(message.data(), message.length());
		}
		secondarySock->cclose();
		delete secondarySock;

		cout << "serverSocketReader - finished" << endl;
	}
	~TCPServer() {
		serverSock->cclose();
		delete serverSock;
	}
};

int main() {
	cout << "TCP Socket test" << endl;

	//create server TCP socket named cSocket
	TCPServer tcpServer(3346);
	tcpServer.start();

	sleep(1); //give a chance for the TCP server to start

	//create client TCP socket named cSocket
	TCPSocket* cSocket = new TCPSocket("127.0.0.1", 3346);

	// this part test the send methods
	message = "Test 1234567890";
	cout << "sending message:" << message << endl;
	int msgLen = htonl(message.length());
	cSocket->send((char*) &msgLen, 4);
	cSocket->send(message.data(), message.length());

	char buffer[100];
	memset((void*) buffer, 0, 100);
	cSocket->recv((char*) &msgLen, 4);
	msgLen = ntohl(msgLen);
	int rc = cSocket->recv(buffer, msgLen);
	cout << "recv msg:" << buffer << endl;
	if (rc != (int) message.length()) {
		perror("FAIL1: receive different message length");
	}
	if (message != buffer) {
		perror("FAIL2: receive different message");
	}
	cout << "client closing" << endl;

	tcpServer.waitForThread();
	cSocket->cclose();
	cout << "test completed" << endl;
	return 0;
}

