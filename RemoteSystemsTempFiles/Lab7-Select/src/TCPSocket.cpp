/*
 * TCPSocket.cpp
 *
 *  Created on: Feb 6, 2013
 *      Author: Eliav Menachi
 */

#include <iostream>
#include <sys/types.h>
#include <netinet/in.h>
#include <inttypes.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <strings.h>
#include <unistd.h>
#include <stdio.h>
#include "TCPSocket.h"
using namespace std;

TCPSocket::TCPSocket(int connected_sock,struct sockaddr_in serverAddr,struct sockaddr_in peerAddr){
	//TODO: initialize local class properties;
}


TCPSocket::TCPSocket(int port){
	//TODO: open TCP socket

	//TODO: set the server address for binding

	//TODO: bind the socket to the address

}


TCPSocket::TCPSocket(string peerIp, int port){
	//TODO: open TCP socket

	//TODO: set the peer address to connect to

	//TODO: connect the socket to the peer server

}


TCPSocket* TCPSocket::listenAndAccept(){
	//TODO: listen on the socket

	//TODO: call accept on the socket

	//TODO: return new TCPSocket object holding the new secondary server socket returned from the accept call

	return NULL;
}


int TCPSocket::recv(char* buffer, int length){
	//TODO: read from the socket

	return -1;
}


int TCPSocket::send(const char* msg, int len){
	//TODO: write on the socket

	return -1;
}


void TCPSocket::cclose(){
	//TODO: shutdown and close the socket
}

string TCPSocket::fromAddr(){
	//TODO: return the session destination peer address

	return "";
}
