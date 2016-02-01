#include "MultipleTCPSocketsListener.h"

using namespace std;

/*
 * Add the given socket to the socket list to be listen on
 */
void MultipleTCPSocketsListener::addSocket(TCPSocket* socket) {
	this->sockets.push_back(socket);
}

/*
 * Set the given sockets to the socket list to be listen on
 */
void MultipleTCPSocketsListener::addSockets(vector<TCPSocket*> socketVec) {
	this->sockets.insert(this->sockets.end(), socketVec.begin(),
			socketVec.end());
}

/*
 * This method checks to see if any of the previous given sockets are ready for reading
 * It returns one of the Sockets that are ready.
 */
TCPSocket* MultipleTCPSocketsListener::listenToSocket() {
	fd_set rfds;
	FD_ZERO(&rfds);
	struct timeval tv;

	tv.tv_sec = 1;
	tv.tv_usec = 0;
	int maxSocketFd = 0;

	// Fill the set with file descriptors from the socket list using (FD_SET macro)
	for (int i = 0; i < this->sockets.size(); i++) {
		FD_SET(sockets[i]->connected_sock, &rfds);

		maxSocketFd =
				maxSocketFd < sockets[i]->connected_sock ?
						sockets[i]->connected_sock : maxSocketFd;
	}

	// Perform the select
	int selectNum = select(maxSocketFd + 1, &rfds, NULL, NULL, &tv);

	if (selectNum < 0) {
		return NULL;
	} else if (selectNum == 0) {
		// Nobody is ready

		return NULL;
	}

	for (int i = 0; i < this->sockets.size(); i++) {
		TCPSocket* socket = sockets[i];

		// Check the returned value from the select to find the socket that is ready
		if (FD_ISSET(socket->connected_sock, &rfds)) {
			return socket;
		}
	}

	return NULL;
}

