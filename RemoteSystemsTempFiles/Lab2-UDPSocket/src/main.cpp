//============================================================================
// Name        : Lab1UDPSocket
// Author      : Eliav Menachi
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <string.h>
#include "UDPSocket.h"
using namespace std;

int main() {
	cout << "UDP Socket test" << endl;
	//create client UDP socket named cSocket
	UDPSocket* cSocket = new UDPSocket();
	//create server UDP socket named sSocket
	UDPSocket* sSocket = new UDPSocket(3346);

	// this part test the sendTo and recv methods
	string message = "Test 1234567890";
	cSocket->sendTo(message,"127.0.0.1", 3346);
	char buffer[100];
	memset((void*)buffer,0,100);
	int rc = sSocket->recv(buffer, 100);
	cout<<"send msg:"<<message<<endl;
	cout<<"recv msg:"<<buffer<<endl;
	if (rc != (int)message.length()){
		perror("FAIL1: receive different message length");
	}
	if (message != buffer){
		perror("FAIL2: receive different message");
	}


	//TODO: Add a test to the reply and recv methods


	//TODO: Close the sockets and delete the socket objects

	cout<<"from address = "<<sSocket->fromAddr()<<endl;
	cout<<"from address = "<<cSocket->fromAddr()<<endl;
	cout<<"TEST COMPLETE"<<endl;
	return 0;
}
