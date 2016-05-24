#pragma once

#include <iostream>
#include <WinSock2.h>
#include <Windows.h>
#include <string>
#include <vector>
#include <map>
#include <mutex>
#include <queue>

#include "Helper.h"
#include "User.h"
#include "TriviaServer.h"
#include "DataBase.h"
#include "RecivedMessage.h"
#include "Game.h"

class Room
{
public:
	Room(int id, User* admin, string name, int maxUsers, int questionNum, int questionTime);
	bool joinRoom(User*);
	void leaveRoom(User*);
	int closeRoom(User*);
	vector<User*> getUsers();
	string getUserListMessage();
	int getQuestionNum();
	int getId();
	string getName();

private:
	vector<User*> _users;
	User* _admin;
	int _maxUsers; 
	int _questionTime;
	int _questionNum;
	string _name;
	int _id;
};

