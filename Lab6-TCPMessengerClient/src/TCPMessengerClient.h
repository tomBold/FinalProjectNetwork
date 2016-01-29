//============================================================================
// Name        : TCP Messenger Server
// Author      : Eliav Menachi
// Version     :
// Copyright   :
// Description : TCP Messenger application
//============================================================================
#pragma once
#include <strings.h>
#include <map>
#include <vector>
#include "MThread.h"
#include <sstream>
#include <iterator>
#include <vector>
#include "TCPSocket.h"
#include "TCPMessengerProtocol.h"
#include "AsyncTCPMessengerReader.h"

using namespace std;

class AsyncTCPMessengerReader;

/**
 * The TCP Messenger client class
 */
class TCPMessengerClient {
	TCPSocket* serverSocket;
	AsyncTCPMessengerReader* reader;
	bool hasConnection;

	/**
	 * Sends a command to the remote server
	 */
	bool sendCommand(int command);

	/**
	 * Sends a command to the remote server
	 */
	bool sendCommand(int command, string data);

public:
	/**
	 * initialize all properties
	 */
	TCPMessengerClient();

	/**
	 * Creates a message byte array from the data received
	 * with the length as an output parameter
	 */
	static char* createMessage(int command, string data,
			int& outLength);

	/**
	 * Converts an int to a byte array
	 */
	static char* intToByteArray(int number);

	/**
	 * Converts a byte array to int
	 */
	static int byteArrayToInt(char* str);

	/**
	 * Handle receiving a message from the server
	 */
	void onServerMessage(int cmd, char* data);

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

	~TCPMessengerClient();

};
