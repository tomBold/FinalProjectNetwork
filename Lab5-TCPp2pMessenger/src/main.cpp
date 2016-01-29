/*
 * main.cpp
 *
 *  Created on: Feb 6, 2013
 *      Author: efi
 */

#ifndef MAIN_CPP_
#define MAIN_CPP_
using namespace std;
#include<sstream>

#include "TCPp2pMessenger.h"

void printInstructions() {
	cout << "To open session type: o <ip>" << endl;
	cout << "Opening session will close any previously active sessions" << endl;
	cout << "After the session is opened, to send message type: s <message>"
			<< endl;
	cout << "To reply to an incoming message from peer type: r <message>"
			<< endl;
	cout << "To close an open session: cs" << endl;
	cout << "To exit type: x" << endl;
}

int test() {
	TCPp2pMessenger p;
	p.open("127.0.0.1");

	p.send("0005hello");
	p.reply("0006shalom");

	sleep(2);
}

string getCommand(string message) {
	int index = message.find(" ");
	return message.substr(0, index);
}

string argument(string message) {
	int index = message.find(" ");
	return message.substr(index + 1);
}

string createMessage(string message) {
	int len = message.length();
	std::ostringstream ss;
	ss << len;
	string lenString = "0000" + ss.str();
	std::string lenSubstring = lenString.substr(lenString.length() - 4);
	return lenSubstring + message;
}

int repl() {
	char userInput[128];
	bool toExit = false;
	TCPp2pMessenger p2p;
	while (!toExit) {
		cin.getline(userInput, sizeof(userInput));
		string command = getCommand(userInput);
		string arg = argument(userInput);
		if (command == "o") {
			cout << "trying to open connection to " << arg << endl;
			p2p.open(arg);
		} else if (command == "s") {
			p2p.send(createMessage(arg));
			cout << "me: " << arg << endl;
		} else if (command == "r") {
			p2p.reply(createMessage(arg));
			cout << "me (reply): " << arg << endl;
		} else if (command == "cs") {
			cout << "CLOSE SESSION" << endl;
			p2p.closeSession();
		} else if (command == "x") {
			cout << "EXIT" << endl;
			toExit = true;
		} else if (command == "t") {
			p2p.send("0005hello");
			p2p.reply("0006shalom");
		} else {
			printInstructions();
		}
	}
}

int main() {
	cout << "Welcome to TCP p2p messenger" << endl;
	printInstructions();
	//test();
	// Read user input and act accordingly
	repl();

	//TODO: on exit close and delete all objects
	cout << "messenger was closed" << endl;
	return 0;
}

#endif /* MAIN_CPP_ */
