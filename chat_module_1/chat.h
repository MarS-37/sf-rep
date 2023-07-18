#pragma once

#include <iostream>
#include <fstream>
#include "user.h"

// подключаем библиотеку для 
// работы с файлами JSON
#include <nlohmann/json.hpp>
using json = nlohmann::json;

class chat
{
public:
	void start()
	{
		// загружаем данные о 
		// пользователях чата
		loadUsers();

		std::cout << "Программа консольный чат." << std::endl;
		std::cout << "Для получения справки введите команду /help" << std::endl;

		while (true) {

			// после авторизации отображаем пользователя
			std::cout << (loggedInUser ? loggedInUser->getLogin() : "") << "> ";
			std::string command;
			std::cin >> command;

			try {
				if (command == "/help") {
					displayHelp();
				} 
				else if (command == "/signup") {
					
				}
				else {
					std::cout << "Неизвестная команда. Используйте /help для помощи." << std::endl;
				}
			}
			catch (const std::exception& e) {
				std::cerr << "Ошибка: " << e.what() << std::endl;
			}
		}
	}

private:
	// контейнер для user
	std::vector<user> users;

	// инициализация объекта
	// контейнера
	user* loggedInUser = nullptr;

	void loadUsers()
	{
		// создаем поток файла
		std::ifstream userFile("users.json");
		if (userFile) { // файл открыт

			// объект JSON
			json userData;

			// получаем данные
			// пользователей
			userFile >> userData;

			//пока есть данные
			for (const auto& user : userData) {
				std::string login = user["userlogin"];
				std::string name = user["username"];
				std::string password = user["userpassword"];
				//добавить цвет пользователя

				// создаем значение JSON и в контейнер
				users.emplace_back(login, name, password);
			}

			userFile.close();
		}
	}

	void saveUsers() {

		// объект JSON
		json userData;

		//пока есть данные
		for (const auto& user : users) {
			userData.push_back({
				{"userlogin", user.getLogin()},
				{"username", user.getName()},
				});
		}

		// создаем поток файла
		std::ofstream userFile("users.json");
		if (userFile) { // файл открыт

			// записываем JSON строку в файл
			userFile << userData.dump(4);

			userFile.close();
		}
	}

	void displayHelp() {
		std::cout << "Список команд:" << std::endl;
		std::cout << "/help - список команд." << std::endl;
		std::cout << " " << std::endl;
		std::cout << " " << std::endl;
		std::cout << " " << std::endl;
		std::cout << " " << std::endl;
		std::cout << " " << std::endl;
	}
};

