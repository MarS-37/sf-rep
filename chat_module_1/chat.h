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

		std::cout << "��������� ���������� ���." << std::endl;
		std::cout << "��� ��������� ������� ������� ������� /help" << std::endl;

		while (true) {

			// ����� ����������� ���������� ������������
			std::cout << (loggedInUser ? loggedInUser->getLogin() : "") << "> ";
			std::string command;
			std::cin >> command;

			try {
				if (command == "/help") {
					displayHelp();
				} 
				else if (command == "/signup") {
					signUp();
				}
				else {
					std::cout << "����������� �������. ����������� /help ��� ������." << std::endl;
				}
			}
			catch (const std::exception& e) {
				std::cerr << "������: " << e.what() << std::endl;
			}
		}
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
				//�������� ���� ������������

				// ������� �������� JSON � � ���������
				users.emplace_back(login, name, password);
			}

			userFile.close();
		}
	}

	void saveUsers() {

		// ������ JSON
		json userData;

		//���� ���� ������
		for (const auto& user : users) {
			userData.push_back({
				{"userlogin", user.getLogin()},
				{"username", user.getName()},
				});
		}

		// ������� ����� �����
		std::ofstream userFile("users.json");
		if (userFile) { // ���� ������

			// ���������� JSON ������ � ����
			userFile << userData.dump(4);

			userFile.close();
		}
	}

	void displayHelp() {
		std::cout << "������ ������:" << std::endl;
		std::cout << "/help - ������ ������." << std::endl;
		std::cout << "/signup - ����������� ������������." << std::endl;
		std::cout << " " << std::endl;
		std::cout << " " << std::endl;
		std::cout << " " << std::endl;
		std::cout << " " << std::endl;
	}

	void signUp() {
		if (loggedInUser) {
			throw std::runtime_error("�� ��� ����� � �������. ����������� /logout.");
		}

		std::string login, name, password;
		std::cout << "���������� ���������� �����: ";
		std::cin >> login;

		// �������� ������ �� ������
		auto it = std::find_if(users.begin(), users.end(), [&](const user& user) {
			return user.getLogin() == login;
			});

		if (it != users.end()) {
			throw std::runtime_error("����� ��� ����������. ����������, ������� ������ �����.");
		}

		std::cout << "������� ���� ���: ";

		// ������������ ������ ����� ������, ���������� � ������ �����
		std::cin.ignore();
		std::getline(std::cin, name);

		std::cout << "������� ������: ";
		std::cin >> password;

		// ������� ������ ��� ������ ������
		user newUser(login, name, password);

		// � ���������
		users.push_back(newUser);

		// ����� � ����
		saveUsers();

		// ������� ���� server@username.json
		std::ofstream userFile("server@" + login + ".json");
		if (userFile) { // ���� ������

			// ������������� ������� 
			// JSON-������� ��� ������������
			userFile << "{}";
			userFile.close();
		}

		std::cout << "����� ������� ������ �������" << std::endl;
	}


};

