//============================================================================
// Name        : TCPMessengerServer
// Author      : Eliav Menachi
// Version     :
// Copyright   :
// Description :
//============================================================================

#include <iostream>
#include "TCPMessengerServer.h"
using namespace std;

void printInstructions(){
	cout<<"-----------------------"<<endl;
	cout<<"lp - list peers"<<endl;
	cout<<"x - shutdown server"<<endl;
	cout<<"-----------------------"<<endl;
}

int main(){
	cout<<"Welcome to TCP messenger Server"<<endl;
	printInstructions();
	TCPMessengerServer msngrServer;
	while(true){
		string msg;
		string command;
		cin >> command;
		if(command == "lp"){
			msngrServer.listPeers();
		}else if(command == "x"){
			break;
		}else{
			cout<<"wrong input"<<endl;
			printInstructions();
		}
	}
	msngrServer.close();
	cout<<"messenger was closed"<<endl;
	return 0;
}
