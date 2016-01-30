/*
 * Strings.h
 *
 *  Created on: Jan 30, 2016
 *      Author: user
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
	static vector<string> split(string data, string token);
};

#endif /* STRINGS_H_ */
