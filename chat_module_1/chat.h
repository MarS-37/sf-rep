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

		std::cout << "Программа консольный чат модуль 1" << std::endl;
		std::cout << "Для получения справки введите команду /help" << std::endl;
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

				// создаем значение JSON и в контейнер
				users.emplace_back(login, name, password);
			}
		}
	}
};

