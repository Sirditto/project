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

/*main server function, contains the main server loop, accepts clients and call to all the other functions in the class*/
void TriviaServer::server()
{
	bindAndListen();
	SOCKET curr_client_soc = INVALID_SOCKET;
	thread *t;

	//main server loop
	while (true)
	{
		//listen for connecting client
		if (listen(_socket, SOMAXCONN) == SOCKET_ERROR) {
			cout << "an error occured while listening for connecting client, error: " + WSAGetLastError() << endl;
			WSACleanup();
		}

		// Accept a client socket
		curr_client_soc = accept(_socket, NULL, NULL);
		if (curr_client_soc == INVALID_SOCKET) {
			printf("accept failed with error: %d\n", WSAGetLastError());
			closesocket(_socket);
			WSACleanup();
		}
		else // case no error occured while accepting the client
		{
			//calling clientHandler() to handle the clients requests.
			t = new thread(clientHandler, curr_client_soc);
			t->join();
		}
		

	}

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

	freeaddrinfo(result);
}

/*handles the request from the messages queue*/
void TriviaServer::handleRecivedMessages()
{
	RecivedMessage* curr_msg = NULL;

	while (true)
	{
		//saving the first message in the queue
		curr_msg = _queRcvMessages.front();

		//locking the mutex
		_mtxRecivedMessages.lock();

		//poping the request
		_queRcvMessages.pop();

		//unlocking the mutex
		_mtxRecivedMessages.unlock();
	}
	
}