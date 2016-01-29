//============================================================================
// Name        : TCP socket test
// Author      : Eliav Menachi
// Version     :
// Copyright   :
// Description : This main tests the MultipleTCPSocketsListener class
//============================================================================

#include <iostream>
#include <string.h>
#include "TCPSocket.h"
#include "MThread.h"
#include "MultipleTCPSocketsListener.h"

// the number of open sockets to test simultaneously
#define TEST_SIZE 20
using namespace std;

string message; // the message sent
bool testFail;  // flag that mark if the test pass or not


/**
 * This is a multiple TCP sockets server
 * it opens TEST_SIZE TCP server sockets, wait for incoming connection on all of them,
 * once there is an incoming connection it perform the following:
 * 	1. connects to it
 * 	2. read the received message
 * 	3. validate the the message is correct (equal to the global message var)
 * 	4. send a new message (also update the global message var so the client will be able to verify it)
 * 	5. close the session socket
 * 	6. wait for the next connection (do this loop TEST_SIZE times after that terminate the server)
 */
class TCPServer:public MThread{
	vector<TCPSocket*> serverSockets; // vector to hold all the sockets

public:
	TCPServer(int port){
		// create the server sockets
		for (int i=0;i<TEST_SIZE;i++){
			TCPSocket* newSock = new TCPSocket(port+i);
			serverSockets.push_back(newSock);
		}
	}

	void run(){
		//the test loop
		for(int j=0;j<TEST_SIZE;j++){
			//TODO: create a MultipleTCPSocketsListener and add the sockets to it

			//TODO: use MultipleTCPSocketsListener to find the socket that is ready

			//TODO: perform listen and accept on the ready socket

			//TODO: receive the incoming message

			//TODO: compare the incoming message to the global message var

			//TODO: create a new message

			//TODO: send the message back to the client

			//TODO: close and delete the new peer socket

		}
	}
	~TCPServer(){
		//close and delete all server sockets
		for (int i=0;i<TEST_SIZE;i++){
			serverSockets[i]->cclose();
			delete serverSockets[i];
		}
	}
};

void createClientAndSendRecvMsg(int port);

int main() {
	cout << "TCP Socket test" << endl;
	//create server socket named sSocket
	testFail = false;
	int port = 55555;
	// create and start the server part
	TCPServer* tcpServer = new TCPServer(port);
	tcpServer->start();
	sleep(2);//give a chance for the TCP server to start

	//call the client test TEST_SIZE times
	for (int i=0;i<TEST_SIZE;i++){
		cout << "testing client #" <<i<< endl;
		createClientAndSendRecvMsg(port+i);
	}

	//wait for the server to close and delete it
	tcpServer->waitForThread();
	delete tcpServer;

	//check if there where any failures
	if (testFail){
		cout<<"!!!!!!!!!! TEST FAIL - look for errors in the log !!!!!!!!!!!"<<endl;
	}else{
		cout<<"********** TEST PASS ***********"<<endl;
	}
	return 0;
}

void createClientAndSendRecvMsg(int port){
	//create client TCP socket named cSocket
	TCPSocket* cSocket1 = new TCPSocket("127.0.0.1",port);

	// this part test the send methods
	message = "Test 1234567890";
	cout<<"sending message:"<<message<<endl;
	int msgLen = htonl(message.length());
	cSocket1->send((char*)&msgLen,4);
	cSocket1->send(message.data(),message.length());

	char buffer[100];
	memset((void*)buffer,0,100);

	MultipleTCPSocketsListener msp;
	msp.addSocket(cSocket1);
	for (int i=0;i<1;i++){
		TCPSocket* readtCSocket = msp.listenToSocket();
		readtCSocket->recv((char*)&msgLen,4);
		msgLen = ntohl(msgLen);
		int rc = readtCSocket->recv(buffer, msgLen);
		cout<<"recv msg:"<<buffer<<endl;
		if (rc != (int)message.length()){
			perror("FAIL1: receive different message length");
			testFail = true;
		}
		if (message != buffer){
			perror("FAIL2: receive different message");
			testFail = true;
		}
	}
	cout<<"client closing"<<endl;
	cSocket1->cclose();
	cout<<"test completed"<<endl;
}
