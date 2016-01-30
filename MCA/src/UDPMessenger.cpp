#include "UDPMessenger.h"
#include <string.h>

using namespace std;

UDPMessenger::UDPMessenger() {
	// Initiate local arguments
	this->isRunning = true;

	// create the socket
	this->mainSocket = new UDPSocket(0);

	// start the thread to receive incoming messages
	this->start();
}

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

void UDPMessenger::handleMsg(string msg, string ipAndPort)
{
	cout << ">[" << this->ipAndPortToUsers[ipAndPort] << "] " << msg << endl;
}

void UDPMessenger::send(string msg) {
	// Send the given message to the given destination
	for (map<string, string>::iterator it = this->ipAndPortToUsers.begin();
			it != this->ipAndPortToUsers.end(); ++it) {
		vector<string> dst = Strings::split(it->first, ":");

		this->mainSocket->sendTo(msg, dst[0], stoi(dst[1]));
	}
}

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

int UDPMessenger::getPort() {
	return this->mainSocket->getPort();
}

UDPMessenger::~UDPMessenger() {
}

void UDPMessenger::setTheMsgDestination(string dst) {
	this->ipAndPortToUsers.clear();
	vector<string> users = Strings::split(dst, ",");

	for (string user : users) {
		vector<string> userData = Strings::split(user, ":");
		this->ipAndPortToUsers[userData[0] + ":" + userData[1]] = userData[2];
	}
}

