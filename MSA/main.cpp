#include <iostream>
#include "src/Utilities/Users.h"

using namespace Utilities;

using namespace std;

// ********** MSA *********//
int main() {
	cout << "lu - list all users" << endl;
	cout << "lcu - list all connected users" << endl;
	cout << "ls - list all sessions (two clients communicating)" << endl;
	cout << "lr - list all rooms" << endl;
	cout << "lru <room name> - list all users in this room" << endl;
	cout << "x - shutdown" << endl;

	//Utilities::Users::H();

}
