#pragma once
#include <string>

class user
{
public:	
	user(const std::string& login, const std::string& name, const std::string& password)
		: f_login(login), f_name(name), f_password(password)
	{
	}

	// геттеры
	std::string getLogin() const
	{
		return f_login;
	}

	std::string getName() const
	{
		return f_name;
	}

	std::string getPassword() const
	{
		return f_password;
	}

private:
	std::string f_login;
	std::string f_name;
	std::string f_password;
};

