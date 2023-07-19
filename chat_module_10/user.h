#pragma once
#include <string>

class user
{
public:

	// конструктор класса
	user(const std::string& login, const std::string& name, const std::string& password);

	// геттеры
	std::string getLogin() const;
	std::string getName() const;
	std::string getPassword() const;

private:
	std::string f_login;
	std::string f_name;
	std::string f_password;
};