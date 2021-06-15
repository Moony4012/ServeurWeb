#pragma once

#include <WS2tcpip.h>
#include <winsock2.h>

#include <string>
#include <vector>
#include <map>

class Client
{
public:

	struct Request
	{
		std::string							_pageName;
		std::map<std::string, std::string>	_arguments;
	};

public:

	Client(SOCKET socket);
	~Client();

	Request ReadRequest();
	void	SendResponse(const std::string& response);

private:

	SOCKET	_socket = INVALID_SOCKET;
};

