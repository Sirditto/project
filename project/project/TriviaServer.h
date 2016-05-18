#pragma once

#include <iostream>
#include <WinSock2.h>
#include <Windows.h>
#include <string>
#include <vector>
#include <map>
#include <mutex>
#include <queue>
#include <thread>

#include "User.h"
#include "DataBase.h"
#include "Room.h"
#include "RecivedMessage.h"

using namespace std;

class TriviaServer
{
public:
	TriviaServer();
	~TriviaServer();

	void server();

private:
	SOCKET _socket;
	map<SOCKET, User*> _connectedUser;
	DataBase _db;
	map<int, Room*> _roomList;
	
	mutex _mtxRecivedMessages;
	queue<RecivedMessage*> _queRcvMessages;

	int static _roomidSequence;

	void bindAndListen();
	void accept();
	void clientHandler(SOCKET);
	void safeDeleteUser(RecivedMessage*);
	
	User* handleSignin(RecivedMessage*);
	bool handleSignup(RecivedMessage*);
	void handleSignout(RecivedMessage*);

	void handleLeaveGame(RecivedMessage*);
	void handleStartGame(RecivedMessage*);
	void handlePlayerAnswer(RecivedMessage*);

	bool handleCreateRoom(RecivedMessage*);
	bool handleCloseRoom(RecivedMessage*);
	bool handleJoinRoom(RecivedMessage*);
	bool handleLeaveRoom(RecivedMessage*);
	void handleGetUserinRoom(RecivedMessage*);
	void handleGetRooms(RecivedMessage*);

	void handleGetBestScores(RecivedMessage*); 
	void handleGetPersonalStatus(RecivedMessage*);

	void handleRecivedMessages();
	void addRecivedMessage(RecivedMessage*);
	RecivedMessage* buildRecivedMessage(SOCKET, int);

	User* getUserByName(string);
	User* getUserBySocket(SOCKET);
	Room* getRoomByid(int);
};

