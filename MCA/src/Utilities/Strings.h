/*
 * Strings.h
 *
 * Provides some utilities with strings
 *
 *  Created on: Jan 30, 2016
 *      Author: Tom Boldan & Gal Schlezinger
 */

#ifndef STRINGS_H_
#define STRINGS_H_

#include <strings.h>
#include <string.h>
#include <string>
#include <vector>

using namespace std;

class Strings {
public:
	/**
	 * Split a string by char
	 */
	static vector<string> split(string data, string token);
};

#endif /* STRINGS_H_ */
