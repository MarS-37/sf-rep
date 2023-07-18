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
					signUp();
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
		std::cout << "/signup - регистрация пользователя." << std::endl;
		std::cout << " " << std::endl;
		std::cout << " " << std::endl;
		std::cout << " " << std::endl;
		std::cout << " " << std::endl;
	}

	void signUp() {
		if (loggedInUser) {
			throw std::runtime_error("Вы уже вошли в систему. Используйте /logout.");
		}

		std::string login, name, password;
		std::cout << "Придумайте уникальный логин: ";
		std::cin >> login;

		// Проверка логина на повтор
		auto it = std::find_if(users.begin(), users.end(), [&](const user& user) {
			return user.getLogin() == login;
			});

		if (it != users.end()) {
			throw std::runtime_error("Логин уже существует. Пожалуйста, введите другой логин.");
		}

		std::cout << "Введите свое имя: ";

		// Игнорировать символ новой строки, оставшийся в буфере ввода
		std::cin.ignore();
		std::getline(std::cin, name);

		std::cout << "Введите пароль: ";
		std::cin >> password;

		// создаем объект для записи учетки
		user newUser(login, name, password);

		// в контейнер
		users.push_back(newUser);

		// пишем в файл
		saveUsers();

		// создаем файл server@username.json
		std::ofstream userFile("server@" + login + ".json");
		if (userFile) { // файл создан

			// Инициализация пустого 
			// JSON-объекта для пользователя
			userFile << "{}";
			userFile.close();
		}

		std::cout << "Новая учетная запись создана" << std::endl;
	}


};

