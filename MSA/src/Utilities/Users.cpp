#include "Users.h"

namespace Utilities {

map<string, string>* Users::getAll() {
	if (userToPassword == NULL) {
		load();
	}

	return userToPassword;
}

bool Users::load() {
	map<string, string>* newUserToPassword = new map<string, string>();

	ifstream users;
	ifstream passwords;

	passwords.open("passwords");
	users.open("users");

	string password;
	string name;

	while (users && passwords) {
		getline(users, name);
		getline(passwords, password);
		newUserToPassword->insert(name, password);
	}

	users.close();
	passwords.close();

	map<string, string>* oldUserToPassword = userToPassword;
	userToPassword = newUserToPassword;

	if (oldUserToPassword != NULL) {
		delete oldUserToPassword;
	}

	return true;
}

bool Users::contains(string name) {
	map<string, string>* all = getAll();
	return all->find(name) != all->end();
}
bool Users::create(string name, string password) {
	if (contains(name)) {
		return false;
	}

	ofstream users;
	ofstream passwords;

	users.open("users", std::ios_base::app);
	passwords.open("passwords", std::ios_base::app);

	users << name;
	passwords << password;

	users.close();
	passwords.close();

	load();

	return true;
}

bool Users::login(string name, string password) {
	if (!contains(name))
	{
		return false;
	}

	return (userToPassword->find(name)->second == password);
}

vector<string> Users::getAllNames() {
	vector<string> names;

	for (std::map<string, string>::const_iterator it = getAll()->begin();
			it != getAll()->end(); it++) {
		string key = it->first;
		names.push_back(key);
	}

	return names;
}

} /* namespace Utilities */
