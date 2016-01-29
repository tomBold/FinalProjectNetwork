

#include "MultipleTCPSocketsListener.h"

using namespace std;


/*
 * Add the given socket to the socket list to be listen on
 */
void MultipleTCPSocketsListener::addSocket(TCPSocket* socket){
	//TODO: add he given socket to the socket list
}

/*
 * Set the given sockets to the socket list to be listen on
 */
void MultipleTCPSocketsListener::addSockets(vector<TCPSocket*> socketVec){
	//TODO: set the given sockets as the socket list to selct from
}

/*
 * This method checks to see if any of the previous given sockets are ready for reading
 * It returns one of the Sockets that are ready.
 */
TCPSocket* MultipleTCPSocketsListener::listenToSocket(){
	//TODO: create local set for the select function (fd_set)

	//TODO: fill the set with file descriptors from the socket list using (FD_SET macro)

	//TODO: perform the select

	//TODO: check the returned value from the select to find the socket that is ready

	//TODO: if select return a valid socket return the matching TCPSocket object otherwise return NULL

	return NULL;
}

