#pragma once
#include <string>

class user
{
public:

	// ����������� ������
	user(const std::string& login, const std::string& name, const std::string& password);

	// �������
	std::string getLogin() const;
	std::string getName() const;
	std::string getPassword() const;

private:
	std::string f_login;
	std::string f_name;
	std::string f_password;
};