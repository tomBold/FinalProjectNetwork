#include <iostream>
#include "src/Utilities/Users.h"
#include "vector"
#include "src/TCPMessengerServer.h"

using namespace Utilities;

using namespace std;

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

	while (true) {
		printInstructions();
		string command = "";
		cin >> command;

		if (command == "lu") {
			vector<string> names = Users::getAllNames();
			for (int i = 0; i < names.size(); i++) {
				cout << names[i] << endl;
			}
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
			}
		} else {
			cout << "Wrong input" << endl;
			printInstructions();
		}
	}
}

