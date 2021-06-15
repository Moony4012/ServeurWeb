#include "Client.h"

Client::Client(SOCKET socket)
	: _socket(socket)
{

}

Client::~Client()
{
	closesocket(_socket);
}

Client::Request Client::ReadRequest()
{
	Request request;

	std::string buffer;
	buffer.reserve(256);

	char staticBuffer[256] = { '\0' };
	int recvLen = recv(_socket, staticBuffer, sizeof(staticBuffer), 0);
	if (recvLen > 0)
	{
		buffer = staticBuffer;
		buffer = buffer.substr(buffer.find(' ') + 1);
		buffer = buffer.substr(0, buffer.find(' '));
		request._pageName = buffer.substr(0, buffer.find('?'));
		buffer = buffer.substr(buffer.find('?') + 1);

		while (buffer.empty() == false)
		{
			size_t endOffset = buffer.find("&");

			std::string name = buffer.substr(0, buffer.find("="));
			std::string value = buffer.substr(buffer.find("=") + 1, endOffset - (buffer.find("=") + 1));
			buffer = buffer.substr(endOffset + 1);

			request._arguments.emplace(name, value);

			if (endOffset == std::string::npos)
			{
				break;
			}
		}
	}
	
	return request;
}

void Client::SendResponse(const std::string& response)
{
	std::string body = 
	"<!doctype html>\r\n"
	"<html>\r\n"
	"\t<head>\r\n"
	"\t\t<title>Password Gen</title>\r\n"
	"\t</head>\r\n"
	"\t<body style=\"background-color: black;\">"

	"\t<form>\r\n"

		"\t<label style=\"color: white;\" for=\"Len\">Length:</label>\r\n"
		"\t<input type=\"number\" id=\"Len\" name=\"Len\" min=\"0\" max=\"255\" value=\"16\" />\r\n"
		"\t<br/>\r\n"

		"\t<input type=\"checkbox\" id=\"UseLowerCase\" name=\"UseLowerCase\" checked />\r\n"
		"\t<label style=\"color: white;\" for=\"UseLowerCase\">Use lower case</label>\r\n"
		"\t<br/>\r\n"

		"\t<input type=\"checkbox\" id=\"UseUpperCase\" name=\"UseUpperCase\" checked />\r\n"
		"\t<label style=\"color: white;\" for=\"UseUpperCase\">Use upper case</label>\r\n"
		"\t<br/>\r\n"

		"\t<input type=\"checkbox\" id=\"UseNumber\" name=\"UseNumber\" checked />\r\n"
		"\t<label style=\"color: white;\" for=\"UseNumber\">Use number</label>\r\n"
		"\t<br/>\r\n"

		"\t<input type=\"checkbox\" id=\"UseSpecialChatacters\" name=\"UseSpecialChatacters\" checked />\r\n"
		"\t<label style=\"color: white;\" for=\"UseSpecialChatacters\">Use special characters</label>\r\n"
		"\t<br/>\r\n"

	"\t<br/>\r\n"
	"\t<button type=\"submit\">Generate</button>\r\n"
	"\t<br/>\r\n"

	"\t</form>\r\n"

	"\t\t<p style=\"color: white;\">" + response + "</p>\r\n"
	"\t</body>\r\n"
	"</html>\r\n";

	std::string header = 
	"HTTP/1.1 200 OK\r\n"
	"Content-Type: text/html; charset=UTF-8\r\n"
	"Content-Length: " + std::to_string(body.size()) + "\r\n"
	"Server: MoonyServer\r\n";
	
	std::string fullResponse = header + "\r\n\r\n" + body;

	send(_socket, fullResponse.c_str(), (int)fullResponse.size(), 0);
}
