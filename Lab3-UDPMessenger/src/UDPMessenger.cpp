//============================================================================
// Name        : UDPMessenger
// Author      : Eliav Menachi
// Version     :
// Copyright   : Your copyright notice
// Description : UDP Messenger application
//============================================================================

#include "UDPMessenger.h"
#include <string.h>

using namespace std;

UDPMessenger::UDPMessenger(OnRecieveClbk* clbk) {
	// Initiate local arguments
	rcvClbk = clbk;
	isRunning = true;

	// create the socket
	mainSocket = new UDPSocket(1341);

	// start the thread to receive incoming messages
	this->start();
}

void UDPMessenger::run() {
	char buffer[100];
	memset((void*) buffer, 0, 100);
	mainSocket->recv(buffer, 100);

	// Receive messages while the messenger is running
	while (isRunning) {
		rcvClbk->handleMessage(buffer);
		mainSocket->recv(buffer, 100);
	}
}

void UDPMessenger::sendTo(string msg, string ip) {
	// Send the given message to the given destination
	mainSocket->sendTo(msg, ip, 1341);
}

void UDPMessenger::reply(string msg) {
	// Send the message to the address of the last received message
	mainSocket->reply(msg);
}

void UDPMessenger::close() {
	// Close the thread
	isRunning = false;

	// Close the socket
	mainSocket->cclose();

	// Wait for thread to exit
	this->waitForThread();

	// Delete and free any allocated resources
	delete rcvClbk;
	delete mainSocket;
}

