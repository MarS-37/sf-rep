#pragma once

#include <iostream>
#include <fstream>
#include "user.h"

// ���������� ���������� ��� 
// ������ � ������� JSON
#include <nlohmann/json.hpp>
using json = nlohmann::json;

class chat
{
public:
	void start()
	{
		// ��������� ������ � 
		// ������������� ����
		loadUsers();

		std::cout << "��������� ���������� ��� ������ 1" << std::endl;
		std::cout << "��� ��������� ������� ������� ������� /help" << std::endl;
	}

private:
	// ��������� ��� user
	std::vector<user> users;

	// ������������� �������
	// ����������
	user* loggedInUser = nullptr;

	void loadUsers()
	{
		// ������� ����� �����
		std::ifstream userFile("users.json");
		if (userFile) { // ���� ������
			
			// ������ JSON
			json userData;

			// �������� ������
			// �������������
			userFile >> userData;

			//���� ���� ������
			for (const auto& user : userData) {
				std::string login = user["userlogin"];
				std::string name = user["username"];
				std::string password = user["userpassword"];

				// ������� �������� JSON � � ���������
				users.emplace_back(login, name, password);
			}
		}
	}
};

