
#include "TCPMessengerClient.h"
#include "TCPMessengerProtocol.h"


/**
 * initialize all properties
 */
TCPMessengerClient::TCPMessengerClient(){

}

/**
 * connect to the given server ip (the port is defined in the protocol header file)
 */
bool TCPMessengerClient::connect(string ip){
	return false;
}

/**
 * return true if connected to the server
 */
bool TCPMessengerClient::isConnected(){
	return false;
}

/**
 * disconnect from messenger server
 */
bool TCPMessengerClient::disconnect(){
	return false;
}

/**
 * open session with the given peer address (ip:port)
 */
bool TCPMessengerClient::open(string address){
	return false;
}

/**
 * return true if a session is active
 */
bool TCPMessengerClient::isActiveClientSession(){
	return false;
}

/**
 * close active session
 */
bool TCPMessengerClient::closeActiveSession(){
	return false;
}

/**
 * send the given message to the connected peer
 */
bool TCPMessengerClient::send(string msg){
	return false;
}

