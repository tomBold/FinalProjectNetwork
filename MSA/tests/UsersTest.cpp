#include <iostream>
#include "../src/Utilities/Users.h"
#include "vector"

using namespace std;

// ********** MSA *********
int usersTest() {

	string userNameAndPassword = "tom 12345";
	int index = userNameAndPassword.find(" ");
	string name = userNameAndPassword.substr(0, index);
	cout << name << endl;
	string password = userNameAndPassword.substr(index + 1);
	cout << password << endl;

	Users::create("test", "test");
	Users::create("tom", "12");
	vector<string> names = Users::getAllNames();
	for (int i = 0; i < names.size(); i++)
	{
		cout << names[i] << endl;
	}

}
