#include <iostream>
#include "src/Utilities/Users.h"
#include "vector"
#include "src/TCPMessengerServer.h"
#include <csignal>
#include <stdlib.h>

using namespace std;

/**
 * We will store the server globally for cleaning it up.
 */
TCPMessengerServer* globalServer = NULL;

/*
 * Handles termination signals for cleanups.
 */
void handleSignal(int signal) {
	try {
		if (globalServer != NULL) {
			globalServer->close();
		}
	} catch (int e) {
	}

	// Kill with the correct exit code
	exit(signal);
}

void printInstructions() {
	cout << "Messenger Server App (MSA)" << endl;
	cout << "*************************" << endl;
	cout << "lu - list all users" << endl;
	cout << "lcu - list all connected users" << endl;
	cout << "ls - list all sessions (two clients communicating)" << endl;
	cout << "lr - list all rooms" << endl;
	cout << "lru <room name> - list all users in this room" << endl;
	cout << "x - shutdown" << endl;
	cout << "help" << endl;
}

// ********** MSA **********
int main() {

	TCPMessengerServer* server = new TCPMessengerServer();
	globalServer = server;

	// Register to SIGTERM, SIGABRT and SIGINT signals
	signal(SIGTERM, handleSignal);
	signal(SIGABRT, handleSignal);
	signal(SIGINT, handleSignal);

	bool isRunning = true;

	while (isRunning) {
		printInstructions();
		string command = "";
		cin >> command;

		if (command == "lu") {
			cout << Users::getAllUsers() << endl;
		} else if (command == "lcu") {
			cout << server->getConnectUsers() << endl;
		} else if (command == "ls") {
			cout << server->getAllBrokers() << endl;
		} else if (command == "lr") {
			cout << server->getRoomsNames() << endl;
		} else if (command == "lru") {
			string roomName = "";
			cin >> roomName;
			cout << server->getRoomsUsers(roomName) << endl;
		} else if (command == "x") {
			server->close();
			isRunning = false;
		} else if (command == "help") {
			printInstructions();
		} else {
			cout << "Wrong input" << endl;
			printInstructions();
		}
	}

	delete globalServer;
}

