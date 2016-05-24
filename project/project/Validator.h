#pragma once
#include <iostream>

using namespace std;

#define DIGITS "0123456789"
#define LOWER "abcdefghijklmnopqrstuvwxyz"
#define UPPER "ABCDEFGHIJKLMNOPQRSTUVWXYZ"

class Validator
{
public:
	static bool isPasswordValid(string password);
	static bool isUsernameValid(string username);
};

