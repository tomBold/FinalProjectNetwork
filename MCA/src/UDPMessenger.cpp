/*
 * UDPMessenger.cpp
 *
 * Handles the peer to peer communication between two (or more) clients
 *
 *  Created on: Jan 30, 2016
 *      Author: Tom Boldan & Gal Schlezinger
 */

#include "UDPMessenger.h"

using namespace std;

UDPMessenger::UDPMessenger() {
	// Initiate local arguments
	this->isRunning = true;

	// create the socket
	this->mainSocket = new UDPSocket(0);

	// start the thread to receive incoming messages
	this->start();
}

/**
 * Read all the incoming messages and print them in the correct format
 */
void UDPMessenger::run() {
	char buffer[100];
	memset((void*) buffer, 0, 100);
	this->mainSocket->recv(buffer, 100);

	// Receive messages while the messenger is running
	while (isRunning) {
		this->handleMsg(buffer, this->mainSocket->destIpAndPort());
		memset((void*) buffer, 0, 100);
		this->mainSocket->recv(buffer, 100);
	}
}

/**
 * Handle a message
 */
void UDPMessenger::handleMsg(string msg, ip_and_port ipAndPort)
{
	cout << ">[" << this->ipAndPortToUsers[ipAndPort] << "] " << msg << endl;
}

/**
 * Sends the given message to the destinations
 */
void UDPMessenger::send(string msg) {
	// Send the given message to the given destination
	for (map<string, string>::iterator it = this->ipAndPortToUsers.begin();
			it != this->ipAndPortToUsers.end(); ++it) {
		vector<string> dst = Strings::split(it->first, ":");

		this->mainSocket->sendTo(msg, dst[0], stoi(dst[1]));
	}
}

/**
 * Close the messenger and stop listening to that port
 */
void UDPMessenger::close() {
	// Close the thread
	isRunning = false;

	// Close the socket
	this->mainSocket->cclose();

	// Wait for thread to exit
	this->waitForThread();

	// Delete and free any allocated resources
	delete mainSocket;
}

/**
 * Get the main socket port
 */
int UDPMessenger::getPort() {
	return this->mainSocket->getPort();
}

UDPMessenger::~UDPMessenger() {
}

/**
 * Set the messages destination
 */
void UDPMessenger::setTheMsgDestination(string dst) {
	this->ipAndPortToUsers.clear();
	if (dst.empty()) {
		return;
	}

	vector<string> users = Strings::split(dst, ",");

	for (string user : users) {
		vector<string> userData = Strings::split(user, ":");
		this->ipAndPortToUsers[userData[0] + ":" + userData[1]] = userData[2];
	}
}
