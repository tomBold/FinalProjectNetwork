/*
 * main.cpp
 *
 *  Created on: Feb 6, 2013
 *      Author: efi
 */

#ifndef MAIN_CPP_
#define MAIN_CPP_

#include "TCPp2pMessenger.h"

void printInstructions(){
	cout<<"To open session type: o <ip>"<<endl;
	cout<<"Opening session will close any previously active sessions"<<endl;
	cout<<"After the session is opened, to send message type: s <message>"<<endl;
	cout<<"To reply to an incoming message from peer type: r <message>"<<endl;
	cout<<"To close an open session: cs"<<endl;
	cout<<"To exit type: x"<<endl;
}

int main(){
	cout<<"Welcome to TCP p2p messenger"<<endl;
	printInstructions();
	//TODO: read user input and act accordingly

	//TODO: on exit close and delete all objects
	cout<<"messenger was closed"<<endl;
	return 0;
}


#endif /* MAIN_CPP_ */
