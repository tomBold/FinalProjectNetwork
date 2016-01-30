#include <iostream>
#include "TCPMessengerProtocol.h"
#include "src/TCPMessengerClient.h"
#include <csignal>
#include <functional>

using namespace std;

// ********** MCA *********//
void handleSignal(TCPMessengerClient* client, int signal) {
	client->disconnect();
	cout << "Bye" << endl;
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
}

int main() {
	TCPMessengerClient* client = new TCPMessengerClient();
	//auto signalHandler = bind(handleSignal, client, placeholders::_1);
	//signal(SIGTERM, signalHandler);
	//signal(SIGABRT, signalHandler);

	bool isRunning = true;
	printInstructions();

	while (isRunning) {
		string command = "";
		cin >> command;

		if (command == "c") {
			string ip = "";
			cin >> ip;

			client->connect(ip);
		} else if (command == "login") {
			string name = "";
			string password = "";
			cin >> name;
			cin >> password;

			client->login(name, password);
		} else if (command == "l") {
			cout << "User status: " << client->getStatus() << endl;
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
		} else {
			printInstructions();
		}
	}
}
