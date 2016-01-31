#include <iostream>
#include "src/Utilities/Users.h"
#include "vector"
#include "src/TCPMessengerServer.h"
#include <csignal>

using namespace std;

TCPMessengerServer* globalServer = NULL;

void handleSignal(int signal) {
	if (globalServer != NULL) {
		globalServer->close();
		delete globalServer;
	}

	cout << "Bye" << endl;
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
}

// ********** MSA *********
int main() {

	TCPMessengerServer* server = new TCPMessengerServer();
	globalServer = server;

	signal(SIGTERM, handleSignal);
	signal(SIGABRT, handleSignal);

	bool isRunning = true;

	while (isRunning) {
		printInstructions();
		string command = "";
		cin >> command;

		if (command == "lu") {
			cout << "Users: " << Users::getAllUsers() << endl;
		} else if (command == "lcu") {
			cout << server->dispatcher->getConnectUsers() << endl;
		} else if (command == "ls") {
			cout << server->dispatcher->getAllBrokers() << endl;
		} else if (command == "lr") {
			cout << server->dispatcher->getRoomsNames() << endl;
		}

		else if (command == "lru") {
			string roomName = "";
			cin >> roomName;
			Room* room = server->dispatcher->roomExists(roomName);

			if (server->dispatcher->roomExists(roomName)) {
				cout << room->getRoomsUsers() << endl;
			} else {
				cout << "Room " + roomName + " does not exist" << endl;
			}
		} else if (command == "x") {
			server->close();
			isRunning = false;
		} else {
			cout << "Wrong input" << endl;
			printInstructions();
		}
	}
}

