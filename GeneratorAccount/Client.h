#pragma once

#include <WS2tcpip.h>
#include <winsock2.h>

#include <string>

class Client
{
public:

	struct Request
	{
		enum Type
		{
			Password,
		};

		Type	_type;
	};

public:

	Client(SOCKET socket);
	~Client();

	Request ReadRequest();
	void	SendResponse(const std::string& response);

private:

	SOCKET	_socket = INVALID_SOCKET;
};

