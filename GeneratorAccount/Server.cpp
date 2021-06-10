#include "Server.h"
#include "Client.h"
#include "Password.h"

#include <iostream>
#pragma comment(lib,"Ws2_32.lib")

Server::Server()
{

}

Server::~Server()
{
	if (_socket != INVALID_SOCKET)
	{
		closesocket(_socket);
	}
	WSACleanup();
}

bool Server::Init(int port)
{
	int result;

	// Initialize WinSock
	WSADATA WSAData;
	result = WSAStartup(MAKEWORD(2, 2), &WSAData);
	if (result != NO_ERROR)
	{
		return false;
	}

	// Create server Socket
	_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (_socket == INVALID_SOCKET)
	{
		return false;
	}

	// Setup NoBlocking Socket
	/*
	u_long mode = 1;
	result = ioctlsocket(_socket, FIONBIO, &mode);
	if (result != NO_ERROR)
	{
		return false;
	}
	*/

	// Bind socket
	SOCKADDR_IN sin;
	sin.sin_addr.s_addr = htonl(INADDR_ANY);
	sin.sin_family = AF_INET;
	sin.sin_port = htons(port);

	result = bind(_socket, (SOCKADDR*)&sin, sizeof(sin));
	if (result != NO_ERROR)
	{
		return false;
	}

	// Listen
	result = listen(_socket, SOMAXCONN);
	if (result != NO_ERROR)
	{
		return false;
	}

	return true;
}

void Server::Listen()
{
	while (true)
	{
		SOCKADDR csin;
		int sizeof_csin = sizeof(csin);
		SOCKET sockClient = accept(_socket, (SOCKADDR*)&csin, &sizeof_csin);
		if (sockClient != INVALID_SOCKET)
		{
			std::cout << "New connection" << std::endl;

			Client client(sockClient);
			Client::Request request = client.ReadRequest();

			if (request._type == Client::Request::Type::Password)
			{
				PasswordGenerator generator;
				generator.SetLength(16);
				generator.SetRules(PasswordGenerator::LowerLetters | PasswordGenerator::UpperLetters);

				client.SendResponse("<b>" + generator.Generate() + "</b>");
			}
			else
			{
				client.SendResponse("Unsupported request");
			}
		}
	}
}
