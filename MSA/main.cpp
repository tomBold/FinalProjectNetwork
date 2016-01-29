#include <iostream>
#include "src/Utilities/Users.h"
#include "vector"

using namespace Utilities;

using namespace std;

// ********** MSA *********
int main() {
	cout << "lu - list all users" << endl;
	cout << "lcu - list all connected users" << endl;
	cout << "ls - list all sessions (two clients communicating)" << endl;
	cout << "lr - list all rooms" << endl;
	cout << "lru <room name> - list all users in this room" << endl;
	cout << "x - shutdown" << endl;


	Users::create("test", "test");
	Users::create("tom", "12");
	vector<string> names = Users::getAllNames();
	for (int i = 0; i < names.size(); i++)
	{
		cout << names[i] << endl;
	}



}
