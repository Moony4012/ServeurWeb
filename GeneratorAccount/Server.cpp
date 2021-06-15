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

			if (true/*request._pageName == "/password"*/)
			{
				PasswordGenerator generator;
				generator.SetLength(std::atoi(request._arguments["Len"].c_str()));
				int ruleMask = 0;
				if (request._arguments["UseNumber"] == "on")
				{
					ruleMask |= PasswordGenerator::Numbers;
				}
				if (request._arguments["UseUpperCase"] == "on")
				{
					ruleMask |= PasswordGenerator::UpperLetters;
				}
				if (request._arguments["UseLowerCase"] == "on")
				{
					ruleMask |= PasswordGenerator::LowerLetters;
				}
				if (request._arguments["UseSpecialChatacters"] == "on")
				{
					ruleMask |= PasswordGenerator::SpecialChatacters;
				}
				generator.SetRules(ruleMask);

				client.SendResponse("<b>" + generator.Generate() + "</b>");
			}
			//else if (request._pageName == "/pseudo")
			//{
			//	client.SendResponse("<b>Toto</b>");
			//}
			else
			{
				client.SendResponse("Unsupported request");
			}
		}
	}
}
