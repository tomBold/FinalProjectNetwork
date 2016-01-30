#ifndef USERS_H_
#define USERS_H_

#include <map>
#include <iostream>
#include <strings.h>
#include <vector>
#include <fstream>

using namespace std;

class Users {
private:
	static map<string, string>* userToPassword;
	static map<string, string>* getAll();
public:
	static bool load();
	static bool contains(string name);
	static bool create(string name, string password);
	static bool login(string name, string password);
	static vector<string> getAllNames();
	static string getAllUsers();
};

#endif /* USERS_H_ */
