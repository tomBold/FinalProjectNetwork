#include <iostream>
#include "src/TCPMessengerProtocol.h"
#include "src/TCPMessengerClient.h"

using namespace std;

// ********** MCA *********//

void printInstructions(int status) {
	if (status == DISCONNECTED) {
		cout << "c <IP> - connect to the server in the given ip" << endl;

	} else {
		if (status == AUTH) {
			cout << "login <user> <password> - login with the user and password"
					<< endl;
			cout
					<< "register <user> <password> - register the new user with the given password and login the user."
					<< endl;
		} else {
			if (status == ROOM || status == BROKER) {
				cout << "cs - disconnect the open session / exit from a room"
						<< endl;
				cout << "s <message> - send a message" << endl;
			}
			cout << "o <username> - open a session with the user" << endl;
			cout << "or <room name> - enter a chat room" << endl;
			cout << "lu - print the user list from the server" << endl;
			cout << "lcu - print the connected users list" << endl;
			cout << "lr - print all rooms" << endl;
			cout << "lru <room name> - print all users in this room" << endl;
			cout << "d - disconnect from server" << endl;
		}
	}

	cout
			<< "l - print the current status of the client (connected to “xyz”/not connected)"
			<< endl;
	cout << "x - close the app" << endl;

}

int main() {
	TCPMessengerClient* client = new TCPMessengerClient();
	bool isRunning = true;
	int status = DISCONNECTED;

	while (isRunning) {
		printInstructions(status);
		string command = "";
		cin >> command;

		if (command == "c") {
			string ip = "";
			cin >> ip;

			client->connect(ip);

		}
		if (command == "x") {
			// TODO
			isRunning = false;
		}

	}
}
