#include "user.h"

// конструктор класса
user::user(const std::string& login, const std::string& name, const std::string& password)
	: f_login(login), f_name(name), f_password(password)
{
}

// геттеры
std::string user::getLogin() const
{
	return f_login;
}

std::string user::getName() const
{
	return f_name;
}

std::string user::getPassword() const
{
	return f_password;
}