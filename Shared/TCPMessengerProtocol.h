#ifndef TCPMESSENGERPROTOCOL_H_
#define TCPMESSENGERPROTOCOL_H_

/**
 * TCP Messenger protocol:
 * all messages are of the format [Command 4 byte int]
 * and optionally data follows in the format [Data length 4 byte int][ Data ]
 */
#define MSNGR_PORT 3346

#define CLOSE_SESSION_WITH_PEER 	1
#define OPEN_SESSION_WITH_PEER 		2 // DATA: user
#define DISCONNECT_FROM_SERVER_REQ	3
#define SEND_MSG_TO_PEER			4
#define SESSION_REFUSED				5
#define SESSION_ESTABLISHED			6
#define LOGIN_REQ					7 // DATA: user password
#define REGISTER_REQ				8 // DATA: user password
#define USER_ALREADY_EXISTS_RES 	9
#define LOGIN_FAILED				10
#define OPEN_ROOM_REQ				11 // DATA: room
#define CLOSE_ROOM_REQ				12
#define JOIN_ROOM_REQ				13 // DATA: room
#define LEAVE_ROOM_REQ				14
#define NEW_MESSAGE_DST_RES			15 // DATA: ip1:port1:name1,ip2:port2:name2,...
#define USER_JOIN_ROOM_RES			16
#define USER_LEAVE_ROOM_RES			17
#define ALREADY_CONNECTED_RES		18
#define USER_BUSY_RES				19
#define ROOM_ALREADY_EXISTS			20
#define SUCCESSFULY_LEFT_ROOM		21
#define CLOSE_ROOM_RES				22
#define SUCCESSFULY_JOIN_ROOM		23 // DATA: room
#define PRINT_ROOMS_USERS_REQ		24
#define PRINT_ROOMS_USERS_RES		25 // DATA: name1,name2,...
#define PRIMT_ROOMS_NAMES_REQ		26
#define PRIMT_ROOMS_NAMES_RES		27 // DATA: room1,room2,...
#define PRINT_CONNECT_USERS_REQ		28
#define PRINT_CONNECT_USERS_RES		29 // DATA: name1,name2,...
#define SHUT_DOWN_RES				30
#define SUCCESSFULY_LOGIN_RES		31
#define PORT_INIT_RES				32 // DATA: port
#define PORT_INIT_REQ				33
#define PORT_INIT_FAILED			34
#define IN_DISPATCHER				35
#define IN_EMPTY_ROOM				36
#define PRINT_USERS_REQ				37
#define PRINT_USERS_RES				38 // DATA: name1,name2,...

#define ROOM						1
#define DISPATCHER					2
#define BROKER						3
#define AUTH						4
#define DISCONNECTED				0

#define TEST_PEER_NAME "test"
#define SESSION_REFUSED_MSG "Connection to peer refused, peer might be busy or disconnected, try again later"

#endif /* TCPMESSENGERPROTOCOL_H_ */
