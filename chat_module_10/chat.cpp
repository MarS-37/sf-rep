#include "chat.h"
#include <filesystem>

void chat::displayHelp()
{
	std::cout << "������ ������:" << std::endl;
	std::cout << "/help - ������ ������." << std::endl;
	std::cout << "/signup - ����������� ������������." << std::endl;
	std::cout << "/signin - ����������� ������������. " << std::endl;
	std::cout << "/logout - ����� ������������." << std::endl;
	std::cout << "/removeaccount - ������� ������������." << std::endl;
	std::cout << "/exit - ����� �� ���������." << std::endl;
}


void chat::start()
{
	// ��������� ������ � 
	// ������������� ����
	loadUsers();

	std::cout << "��������� ���������� ���." << std::endl;
	std::cout << "��� ��������� ������� ������� ������� /help" << std::endl;

	while (true) {

		// ����� ����������� ���������� ������������
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

					// ������
					// @username ��������� ����������� ������������ username
					// ��� ��������� ����� ���
				if (loggedInUser != nullptr)
					std::cout << "����������� ������������ " + input_text << std::endl;
				else
					std::cout << "����������� �������. ����������� /help ��� ������." << std::endl;

			}
		}
		catch (const std::exception& e) {
			std::cerr << "������: " << e.what() << std::endl;
		}
	}
}


void chat::loadUsers()
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


void chat::saveUsers()
{

	// ������ JSON
	json userData;

	//���� ���� ������
	for (const auto& user : users) {
		userData.push_back({
			{"userlogin", user.getLogin()},
			{"username", user.getName()},
			{"userpassword", user.getPassword()}
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


void chat::signUp()
{
	if (loggedInUser) {
		throw std::runtime_error("����������� ������������ " +
			loggedInUser->getLogin() + ". ����������� ������� /logout.");
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


void chat::signIn()
{
	if (loggedInUser) {
		throw std::runtime_error("��������� ������������ " +
			loggedInUser->getLogin() + ". ����������� ������� /logout.");
	}

	std::string login, password;
	std::cout << "Enter your login: ";
	std::cin  >> login;

	auto it = std::find_if(users.begin(), users.end(), [&](const user& _user) {
		return _user.getLogin() == login;
		});

	if (it == users.end()) {
		throw std::runtime_error("������������ � ����� ������� �� ���������������.");
	}

	std::cout << "������� ������: ";
	std::cin >> password;

	if (it->getPassword() != password) {
		throw std::runtime_error("�������� ������");
	}

	// ����� ������ ������� user
	loggedInUser = &(*it);
	std::cout << "����������� �������, ������������ " << loggedInUser->getName()
		<< "!" << std::endl;
}


void chat::logout()
{
	if (!loggedInUser) {
		throw std::runtime_error("�� �� ����� � �������");
	}

	// ����������� ���������
	std::cout << "������������ " + loggedInUser->getLogin() + " ����� �� ����\n";
	loggedInUser = nullptr;
	
}


void chat::removeAccount()
{
	if (!loggedInUser) {
		throw std::runtime_error("������������ �������������.");
	}

	std::string login = loggedInUser->getLogin();
	users.erase(std::remove_if(users.begin(), users.end(), [&](const user& _user) {
		return _user.getLogin() == login;
		}), users.end());

	saveUsers();

	// ������� ���� ������������
	std::remove(("server@" + login + ".json").c_str());

	// ����������� ���������
	loggedInUser = nullptr;
}