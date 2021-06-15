
#include <iostream>

#include "Password.h"

#include <cstdlib>
#include <ctime>

using namespace std;

#include "Server.h"

int main()
{
	srand(time(0));

	Server server;

	if (server.Init(80) == false)
	{
		return EXIT_FAILURE;
	}

	server.Listen();

	return EXIT_SUCCESS;
}

/*
	PasswordGenerator generator;
	generator.SetLength(16);
	generator.SetRules(PasswordGenerator::LowerLetters | PasswordGenerator::UpperLetters);
	
	for (int i = 0; i < 15; ++i)
	{
		std::string password = generator.Generate();
		
		std::cout << password << std::endl;
	}

	generator.SetRules(PasswordGenerator::LowerLetters | PasswordGenerator::UpperLetters | PasswordGenerator::Numbers);

	std::cout << std::endl;

	for (int i = 0; i < 15; ++i)
	{
		std::string password = generator.Generate();

		std::cout << password << std::endl;
	}

	generator.SetRules(PasswordGenerator::SpecialChatacters);

	std::cout << std::endl;

	for (int i = 0; i < 15; ++i)
	{
		std::string password = generator.Generate();

		std::cout << password << std::endl;
	}
	
	return 0;
}
*/
