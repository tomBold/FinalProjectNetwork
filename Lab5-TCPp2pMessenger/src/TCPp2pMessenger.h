//============================================================================
// Name        : UDPMessenger.h
// Author      : Eliav Menachi
// Version     :
// Copyright   : Your copyright notice
// Description : TCP p2p Messenger application
//============================================================================

#include <pthread.h>
#include "TCPSocket.h"
#include "MThread.h"

using namespace std;

class P2PClient: public MThread {
public:
	TCPSocket* sock;

	P2PClient(string peerIp, int port) {
		this->sock = new TCPSocket(peerIp, port);
	}

	void run() {
		if (sock != NULL) {
			cout << "client is running" << endl;
			char message[2000];

			while (sock->recv(message, 2000) > 0) {
				cout << "client got: " << message << endl;
			}
		}
		cout << "worker - finished" << endl;
	}

	~P2PClient() {
		if (this->sock != NULL) {
			this->sock->cclose();
			delete this->sock;
		}
	}
};

class P2PServer: public MThread {

public:
	TCPSocket* sock;
	TCPSocket* sockSecond;

	P2PServer(int port) {
		this->sock = new TCPSocket(port);
	}

	void run() {
	 sockSecond = sock->listenAndAccept();

		if (sockSecond != NULL) {
			char message[2000];

			while (sockSecond->recv(message, 2000) > 0) {
				cout << "server got: " << message << endl;
			}
		}
		cout << "worker - finished" << endl;
	}

	~P2PServer() {
		if (this->sock != NULL) {
			this->sock->cclose();
			delete this->sock;
		}
		if(this->sockSecond != NULL)
		{
			this->sockSecond->cclose();
			delete this->sockSecond;
		}
	}
};

class TCPp2pMessenger {
	P2PServer* p2pServer;
	P2PClient* p2pClient;

public:
	/**
	 * Construct the P2P messenger - this action include:
	 * 								 opening a TCP server socket
	 * 								 opening a thread that accept all incoming connection and reads
	 * 								 incoming messages
	 */
	TCPp2pMessenger();

	/**
	 * Open a connection to the given IP address and once the connection is
	 * established it will open a thread
	 * to read incoming messages from the socket
	 */
	bool open(string peerIp);

	/**
	 * Close an open session
	 */
	bool closeSession();

	/**
	 * send a message on the client open connection
	 * The message is sent in the form: [message length - 4 bytes][message data]
	 */
	void send(string msg);

	/**
	 * send a message on the server socket incoming connection
	 * The message is sent in the form: [message length - 4 bytes][message data]
	 */
	void reply(string msg);

	/**
	 * close all resources / sockets
	 */
	void close();

	/**
	 * return  true if the client socket is connected
	 */
	bool isActiveClientSession();

	/**
	 * return true if there is an open incoming connection
	 */
	bool isActivePeerSession();
};

