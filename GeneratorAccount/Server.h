#pragma once

#include <WS2tcpip.h>
#include <winsock2.h>

class Server
{
public:

			Server();
			~Server();

	bool	Init(int port);
	void	Listen();

private:

	SOCKET	_socket = INVALID_SOCKET;
};

