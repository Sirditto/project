#pragma once

#include <iostream>
#include <WinSock2.h>
#include <Windows.h>
#include <string>
#include <vector>
#include <map>
#include <mutex>
#include <queue>

#include "TriviaServer.h"
#include "DataBase.h"
#include "Room.h"
#include "RecivedMessage.h"
#include "Game.h"

class User
{
public:
	User(string, SOCKET);
	void send(string);
	string getUsername();
	SOCKET getSocket();
	


private:
	string _username;
	Room* _currRoom;
	Game* _currGame;
	SOCKET _sock;

};

