#include "TriviaServer.h"


/*constructor, initialize the main listening socket */
TriviaServer::TriviaServer() : _socket(INVALID_SOCKET)
{
	//start the database constructor
}


/*distructor, closes the socket and cleans upthe room and user lists*/
TriviaServer::~TriviaServer()
{
	//clearing the rook list
	_roomList.clear();
	//clearing the user list
	_connectedUser.clear();

	//closing the socket
	closesocket(_socket);
	WSACleanup();
}

void TriviaServer::server()
{
	bindAndListen();

	while (true)
	{

	}

}