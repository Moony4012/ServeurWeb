#include "Password.h"
#include <cstdlib>
#include <vector>
#include <functional>

const char PasswordGenerator::_specialCharacters[] = {
	'!', '"', '#', '$', '%', '&', '\'', '(', ')', '*', '/', '+', '-', '.', ':',
	';', '=', '?', '@', '[', '\\', ']', '^', '_', '`', '{', '|', '}', '~'
};

PasswordGenerator::PasswordGenerator()
{
	_length = 16;
	_rules = Rule::Numbers | Rule::LowerLetters | Rule::UpperLetters | Rule::SpecialChatacters;
}

void PasswordGenerator::SetLength(int length)
{
	_length = length;
}

void PasswordGenerator::SetRules(int rules)
{
	_rules = rules;
}

char PasswordGenerator::RandomChar(char min, char max)
{
	return rand() % (max - min + 1) + min;
}

char PasswordGenerator::RandomNumbers()
{
	return RandomChar('0', '9');
}

char PasswordGenerator::RandomLowerLetter()
{
	return RandomChar('a', 'z');
}

char PasswordGenerator::RandomUpperLetter()
{
	return RandomChar('A', 'Z');
}

char PasswordGenerator::RandomSpecialCharacter()
{
	return _specialCharacters[rand() % sizeof(_specialCharacters)];
}

std::string PasswordGenerator::Generate()
{
	std::vector<RandomCharacterFunction> functions;

	if (_rules & Rule::Numbers)
	{
		functions.push_back(&PasswordGenerator::RandomNumbers);
	}

	if (_rules & Rule::LowerLetters)
	{
		functions.push_back(&PasswordGenerator::RandomLowerLetter);
	}

	if (_rules & Rule::UpperLetters)
	{
		functions.push_back(&PasswordGenerator::RandomUpperLetter);
	}

	if (_rules & Rule::SpecialChatacters)
	{
		functions.push_back(&PasswordGenerator::RandomSpecialCharacter);
	}

	if (_rules & Rule::Names)
	{
		functions.push_back(&PasswordGenerator::RandomSpecialCharacter);
	}

	std::string password;

	for (size_t i = 0; i < _length; i++)
	{
		int random = rand() % functions.size();

		password += functions[random]();
	}

	return password;
}
