#include "TriviaServer.h"


/*constructor, initialize the main listening socket */
TriviaServer::TriviaServer() : _socket(INVALID_SOCKET)
{
	_db = *(new DataBase());
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

/*main server function, contains the main server loop, accepts clients and call to all the other functions in the class*/
void TriviaServer::server()
{
	bindAndListen();
	thread t(&handleRecivedMessages);
	t.detach();

	//main server loop
	while (true)
	{
		acceptClient();
	}

}
/*accept a new client*/
void TriviaServer::acceptClient()
{
	SOCKET curr_client_soc = INVALID_SOCKET;

	// Accept a client socket
	curr_client_soc = accept(_socket, NULL, NULL);
	if (curr_client_soc == INVALID_SOCKET) {
		printf("accept failed with error: %d\n", WSAGetLastError());
		closesocket(_socket);
		WSACleanup();
	}

	thread t(&clientHandler, curr_client_soc);
}

/*binds and listens t*/
void TriviaServer::bindAndListen()
{
	WSADATA wsaData;
	int iResult;

	SOCKET ClientSocket = INVALID_SOCKET;

	struct addrinfo *result = NULL;
	struct addrinfo hints;

	int iSendResult;
	char recvbuf[DEFAULT_BUFLEN];
	int recvbuflen = DEFAULT_BUFLEN;

	// Initialize Winsock
	iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != 0) {
		
	}

	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	hints.ai_flags = AI_PASSIVE;

	// Resolve the server address and port
	iResult = getaddrinfo(NULL, DEFAULT_PORT, &hints, &result);
	if (iResult != 0) {
		
	}

	// Create a SOCKET for connecting to server
	_socket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
	if (_socket == INVALID_SOCKET) {
		freeaddrinfo(result);
		WSACleanup();
	}

	// Setup the TCP listening socket
	iResult = bind(_socket, result->ai_addr, (int)result->ai_addrlen);
	if (iResult == SOCKET_ERROR) {
		freeaddrinfo(result);
		WSACleanup();
	}

	//listen for connecting client
	if (listen(_socket, SOMAXCONN) == SOCKET_ERROR) {
		cout << "an error occured while listening for connecting client, error: " + WSAGetLastError() << endl;
		WSACleanup();
	}

	freeaddrinfo(result);
}

/*handles the request from the messages queue*/
void TriviaServer::handleRecivedMessages()
{
	RecivedMessage* curr_msg = NULL;

	while (true)
	{
		//wait untill queue has requests in it
		while (!_queRcvMessages.size());

		//saving the first message in the queue
		curr_msg = _queRcvMessages.front();

		//locking the mutex
		unique_lock<mutex> lck(_mtxRecivedMessages);
		lck.lock();

		//poping the request
		_queRcvMessages.pop();

		//unlocking the mutex
		lck.unlock();

		//linking the username to the message
		curr_msg->setUser(getUserBySocket(curr_msg->getSock()));

	/**************************************************************************************************/

		//if user requests to leave
		if (curr_msg->getMessageCode() == QUIT_REQUEST)
			safeDeleteUser(curr_msg); // delete user

		switch (curr_msg->getMessageCode())
		{
		//case user requests to sign in
		case SIGN_IN_REQUEST:
			handleSignin(curr_msg);
			break;

		//case user requests to sign up
		case SIGN_UP_REQUEST:
			handleSignup(curr_msg);
			break;

		//case user requests to sign out
		case SIGN_OUT_REQUEST:
			handleSignout(curr_msg);

		//case user requests room list
		case ROOM_LIST_REQUEST:
			handleGetRooms(curr_msg);
			break;

		//case user requests list of users in room
		case USER_IN_ROOM_REQUEST:
			handleGetUserinRoom(curr_msg);
			break;

		//case user requests to join an existing room
		case JOIN_ROOM_REQUEST:
			handleJoinRoom(curr_msg);
			break;

		//case user requests to leave room
		case LEAVE_ROOM_REQUEST:
			handleLeaveRoom(curr_msg);
			break;

		//case user requests to create room
		case CREATE_ROOM_REQUEST:
			handleCreateRoom(curr_msg);
			break;

		//case user requests to close room
		case CLOSE_ROOM_REQUEST:
			handleCloseRoom(curr_msg);
			break;

		//case user requests to start game 
		case START_GAME_REQUEST:
			handleStartGame(curr_msg);
			break;

		//case user

		}
	}
}