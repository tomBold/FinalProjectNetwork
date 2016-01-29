#include <iostream>
using namespace std;

// ********** MCA *********//
int main() {
	cout << "c <IP> - connect to the server in the given ip" << endl;
	cout << "lu - print the user list from the server" << endl;
	cout << "lcu - print the connected users list" << endl;
	cout << "lr - print all rooms" << endl;
	cout << "lru <room name> - print all users in this room" << endl;
	cout << "login <user> <password> - login with the user and password"
			<< endl;
	cout
			<< "register <user> <password> - register the new user with the given password and login the user."
			<< endl;
	cout << "o <username> - open a session with the user" << endl;
	cout << "or <room name> - enter a chat room" << endl;
	cout << "s <message> - send a message" << endl;
	cout
			<< "l - print the current status of the client (connected to “xyz”/not connected)"
			<< endl;
	cout << "cs - disconnect the open session / exit from a room" << endl;
	cout << "d - disconnect from server" << endl;
	cout << "x - close the app" << endl;
}
