/*
 * Users.h
 *
 * Wraps the "users" and "passwords" files in a simple class form
 *
 *  Created on: Jan 30, 2016
 *      Author: Tom Boldan & Gal Schlezinger
 */

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

	/**
	 * Caches the file system calls
	 */
	static map<string, string>* getAll();
public:
	/**
	 * Loads the text files to the map
	 */
	static bool load();

	/**
	 * Check if user exists
	 */
	static bool contains(string name);

	/**
	 * Create a new user
	 */
	static bool create(string name, string password);

	/**
	 * Check if there is a match with a username and password
	 */
	static bool login(string name, string password);

	/**
	 * Get all the users registered
	 */
	static vector<string> getAllNames();

	/**
	 * Get all the users registered in a textual representation
	 */
	static string getAllUsers();
};

#endif /* USERS_H_ */
