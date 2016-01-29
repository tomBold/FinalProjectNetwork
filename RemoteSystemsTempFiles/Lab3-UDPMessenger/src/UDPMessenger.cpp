//============================================================================
// Name        : UDPMessenger
// Author      : Eliav Menachi
// Version     :
// Copyright   : Your copyright notice
// Description : UDP Messenger application
//============================================================================

#include "UDPMessenger.h"
#include <string.h>

using namespace std;


UDPMessenger::UDPMessenger(OnRecieveClbk* clbk){
	//TODO: initiate local arguments (if needed)
	rcvClbk = clbk;
	isRunning = true;

	//TODO: create the socket
	mainSocket = new UDPSocket(1341);

	//TODO: start the thread to receive incoming messages
	this->start();
}

void UDPMessenger::run(){
	//TODO: create a loop in which you read from the socket and print the received messages

	char buffer[100];
	memset((void*)buffer,0,100);

	// TODO: 1?
	while(isRunning)
	{
		// Print the received messages
		mainSocket->recv(buffer,100);
		if(isRunning)
		{
		cout << "Receive message from: " << mainSocket->fromAddr() << endl;
		cout << ">" << '"' << buffer << '"' << endl;
		}
	}
}

void UDPMessenger::sendTo(string msg,string ip){
	//TODO: Check port???
	//TODO: send the given message to the given destination
	mainSocket->sendTo(msg, ip, 1341);
}

void UDPMessenger::reply(string msg){
	//TODO: send the message to the address of the last received message
	mainSocket->reply(msg);
}

void UDPMessenger::close(){
	isRunning = false;

	//TODO: close the thread
	//this->;

	//TODO: close the socket
	mainSocket->cclose();

	//TODO: wait for thread to exit
	this->waitForThread();


	//TODO: delete and free any allocated resources
	delete rcvClbk;

}

UDPMessenger::~UDPMessenger(){
this->close();

}




