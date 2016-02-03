#include <iostream>
#include "TCPMessengerProtocol.h"
#include "src/TCPMessengerClient.h"
#include <csignal>
#include <functional>
#include <stdlib.h>

using namespace std;

TCPMessengerClient* globalClient = NULL;

// ********** MCA *********//
void handleSignal(int signal) {
	try {
		if (globalClient != NULL) {
			globalClient->disconnect();
		}
	} catch (int e) {
	}

	exit(signal);
}

void printInstructions() {
	cout << "c <IP> - connect to the server in the given ip" << endl;
	cout << "login <user> <password> - login with the user and password"
			<< endl;
	cout
			<< "register <user> <password> - register the new user with the given password and login the user."
			<< endl;
	cout << "cs - disconnect the open session / exit from a room" << endl;
	cout << "s <message> - send a message" << endl;
	cout << "o <username> - open a session with the user" << endl;
	cout << "or <room name> - enter a chat room" << endl;
	cout << "lu - print the user list from the server" << endl;
	cout << "lcu - print the connected users list" << endl;
	cout << "lr - print all rooms" << endl;
	cout << "lru <room name> - print all users in this room" << endl;
	cout << "d - disconnect from server" << endl;
	cout
			<< "l - print the current status of the client (connected to “xyz”/not connected)"
			<< endl;
	cout << "x - close the app" << endl;
	cout << "help" << endl;
}

int main() {
	TCPMessengerClient* client = new TCPMessengerClient();
	globalClient = client;

	// Set the terminate signal and the abort signal
	// to actually disconnect
	signal(SIGTERM, handleSignal);
	signal(SIGABRT, handleSignal);
	signal(SIGINT, handleSignal);

	bool isRunning = true;
	printInstructions();

	while (isRunning) {
		string command = "";
		cin >> command;

		if (command == "c") {
			string ip = "";
			cin >> ip;

			if (!client->connect(ip))
			{
				printInstructions();
			}
		} else if (command == "login") {
			string name = "";
			string password = "";
			cin >> name;
			cin >> password;

			client->login(name, password);
		} else if (command == "l") {
			client->printStatus();
		} else if (command == "register") {
			string name = "";
			string password = "";
			cin >> name;
			cin >> password;

			client->registerUser(name, password);
		} else if (command == "x") {
			isRunning = false;
			client->disconnect();
		} else if (command == "lu") {
			client->getAllUsers();
		} else if (command == "lcu") {
			client->getConnectedUsers();
		} else if (command == "lr") {
			client->getAllRooms();
		} else if (command == "lru") {
			string room = "";
			cin >> room;

			client->getRoomsUsers(room);
		} else if (command == "d") {
			client->disconnect();
		} else if (command == "cs") {
			client->closeActiveSession();
		} else if (command == "o") {
			string userName = "";
			cin >> userName;

			client->openSession(userName);
		} else if (command == "or") {
			string room;
			cin >> room;

			client->joinRoom(room);
		} else if (command == "s") {
			string msg = "";
			getline(cin, msg);

			client->send(msg);
		} else if (command == "help") {
			printInstructions();
		} else {
			cout << "Wrong input" << endl;
			printInstructions();
		}
	}

	delete globalClient;
}
