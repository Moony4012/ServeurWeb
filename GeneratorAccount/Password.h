#pragma once

#include <string>
#include <functional>

class PasswordGenerator
{
public:

	enum Rule
	{
		Numbers				= (1 << 0),
		LowerLetters		= (1 << 1),
		UpperLetters		= (1 << 2),
		SpecialChatacters	= (1 << 3),
		Names				= (1 << 4),
	};

public:

	PasswordGenerator();

	void SetLength(int length);
	void SetRules(int rules);

	std::string Generate();

private:

	static char RandomChar(char min, char max);

	static char RandomNumbers();
	static char RandomLowerLetter();
	static char RandomUpperLetter();
	static char RandomSpecialCharacter();

private:

	using RandomCharacterFunction = std::function<char()>;

private:

	int _length;
	int _rules;

	static const char _specialCharacters[];

};

