//============================================================================
// Name        : TCP Messenger Server
// Author      : Eliav Menachi
// Version     :
// Copyright   :
// Description : TCP Messenger application
//============================================================================
#include <strings.h>
#include <map>
#include <vector>
#include "MThread.h"
#include "TCPSocket.h"
#include "TCPMessengerProtocol.h"


using namespace std;

/**
 * The TCP Messenger client class
 */
class TCPMessengerClient{
	//TODO: add class properties

public:
	/**
	 * initialize all properties
	 */
	TCPMessengerClient();

	/**
	 * connect to the given server ip (the port is defined in the protocol header file)
	 */
	bool connect(string ip);

	/**
	 * return true if connected to the server
	 */
	bool isConnected();

	/**
	 * disconnect from messenger server
	 */
	bool disconnect();

	/**
	 * open session with the given peer address (ip:port)
	 */
	bool open(string address);

	/**
	 * return true if a session is active
	 */
	bool isActiveClientSession();

	/**
	 * close active session
	 */
	bool closeActiveSession();

	/**
	 * send the given message to the connected peer
	 */
	bool send(string msg);

};

