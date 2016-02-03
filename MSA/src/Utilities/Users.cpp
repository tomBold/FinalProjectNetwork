/*
 * Users.cpp
 *
 * Wraps the "users" and "passwords" files in a simple class form
 *
 *  Created on: Jan 30, 2016
 *      Author: Tom Boldan & Gal Schlezinger
 */

#include "Users.h"

map<string, string>* Users::userToPassword = NULL;

/**
 * Caches the file system calls
 */
map<string, string>* Users::getAll() {
	if (userToPassword == NULL) {
		load();
	}

	return userToPassword;
}

/**
 * Loads the text files to the map
 */
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

		(*newUserToPassword)[name] = password;
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

/**
 * Check if user exists
 */
bool Users::contains(string name) {
	map<string, string>* all = getAll();
	return all->find(name) != all->end();
}

/**
 * Create a new user
 */
bool Users::create(string name, string password) {
	if (contains(name)) {
		return false;
	}

	ofstream users;
	ofstream passwords;

	users.open("users", std::ios_base::app);
	passwords.open("passwords", std::ios_base::app);

	users << endl << name;
	passwords << endl << password;

	users.close();
	passwords.close();

	load();

	return true;
}

/**
 * Check if there is a match with a username and password
 */
bool Users::login(string name, string password) {
	if (!contains(name)) {
		return false;
	}

	return (userToPassword->find(name)->second == password);
}

/**
 * Get all the users registered
 */
vector<string> Users::getAllNames() {
	vector<string> names;

	for (std::map<string, string>::const_iterator it = getAll()->begin();
			it != getAll()->end(); it++) {
		string key = it->first;
		names.push_back(key);
	}

	return names;
}

/**
 * Get all the users registered in a textual representation
 */
string Users::getAllUsers() {
	string result = "";

	vector<string> names = getAllNames();

	if (names.size() == 0) {
		return "There is no users.";
	}

	for (unsigned int i = 0; i < names.size(); i++) {
		if (result != "") {
			result += ",";
		}

		result += names[i];
	}

	return "Users: " + result;
}

