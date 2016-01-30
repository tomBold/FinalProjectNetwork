/*
 * Strings.cpp
 *
 *  Created on: Jan 30, 2016
 *      Author: user
 */

#include "Strings.h"

vector<string> Strings::split(string data, string delimiter)
{
	 vector<string> result;
	 size_t position = 0;
	 string sub;

	 while ((position = data.find(delimiter)) != string::npos) {
		 sub = data.substr(0, position);
	     result.push_back(sub);
	     data.erase(0, position + delimiter.length());
	 }

	 result.push_back(data);

	 return result;
}
