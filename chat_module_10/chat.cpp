#include "chat.h"
#include <filesystem>

void chat::displayHelp()
{
	std::cout << "Список команд:" << std::endl;
	std::cout << "/help - список команд." << std::endl;
	std::cout << "/signup - регистрация пользователя." << std::endl;
	std::cout << "/signin - авторизация пользователя. " << std::endl;
	std::cout << "/logout - выход пользователя." << std::endl;
	std::cout << "/removeaccount - удалить пользователя." << std::endl;
	std::cout << "/exit - выход из программы." << std::endl;
}


void chat::start()
{
	// загружаем данные о 
	// пользователях чата
	loadUsers();

	std::cout << "Программа консольный чат." << std::endl;
	std::cout << "Для получения справки введите команду /help" << std::endl;

	while (true) {

		// после авторизации отображаем пользователя
		std::cout << (loggedInUser ? loggedInUser->getLogin() : "") << "> ";
		std::string input_text;
		std::cin >> input_text;

		try {
			if (input_text == "/help") {
				displayHelp();
			}
			else if (input_text == "/signup") {
				signUp();
			}
			else if (input_text == "/signin") {
				signIn();
			}
			else if (input_text == "/logout") {
				logout();
			}
			else if (input_text == "/removeaccount") {
				removeAccount();
			}
			else if (input_text == "/exit") {
				break;
			}
			else {

					// парсер
					// @username сообщение конкретному пользователю username
					// все остальное общий чат
				if (loggedInUser != nullptr)
					std::cout << "Авторизован пользователь " + input_text << std::endl;
				else
					std::cout << "Неизвестная команда. Используйте /help для помощи." << std::endl;

			}
		}
		catch (const std::exception& e) {
			std::cerr << "Ошибка: " << e.what() << std::endl;
		}
	}
}


void chat::loadUsers()
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


void chat::saveUsers()
{

	// объект JSON
	json userData;

	//пока есть данные
	for (const auto& user : users) {
		userData.push_back({
			{"userlogin", user.getLogin()},
			{"username", user.getName()},
			{"userpassword", user.getPassword()}
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


void chat::signUp()
{
	if (loggedInUser) {
		throw std::runtime_error("Авторизован пользователь " +
			loggedInUser->getLogin() + ". Используйте команду /logout.");
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


void chat::signIn()
{
	if (loggedInUser) {
		throw std::runtime_error("Залогинен пользователь " +
			loggedInUser->getLogin() + ". Используйте команду /logout.");
	}

	std::string login, password;
	std::cout << "Enter your login: ";
	std::cin  >> login;

	auto it = std::find_if(users.begin(), users.end(), [&](const user& _user) {
		return _user.getLogin() == login;
		});

	if (it == users.end()) {
		throw std::runtime_error("Пользователя с таким логином не зарегистрирован.");
	}

	std::cout << "Введите пароль: ";
	std::cin >> password;

	if (it->getPassword() != password) {
		throw std::runtime_error("Неверный пароль");
	}

	// адрес памяти объекта user
	loggedInUser = &(*it);
	std::cout << "Авторизация успешна, здравствуйте " << loggedInUser->getName()
		<< "!" << std::endl;
}


void chat::logout()
{
	if (!loggedInUser) {
		throw std::runtime_error("Вы не вошли в систему");
	}

	// освобождаем указатель
	std::cout << "Пользователь " + loggedInUser->getLogin() + " вышел из чата\n";
	loggedInUser = nullptr;
	
}


void chat::removeAccount()
{
	if (!loggedInUser) {
		throw std::runtime_error("Пользователь неавторизован.");
	}

	std::string login = loggedInUser->getLogin();
	users.erase(std::remove_if(users.begin(), users.end(), [&](const user& _user) {
		return _user.getLogin() == login;
		}), users.end());

	saveUsers();

	// удаляем файл пользователя
	std::remove(("server@" + login + ".json").c_str());

	// освобождаем указатель
	loggedInUser = nullptr;
}