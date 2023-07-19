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
	void start();

private:
	std::vector<user> users;
	user* loggedInUser = nullptr;

	void displayHelp();
	void loadUsers();
	void saveUsers();	
	void signUp();
	void signIn();
	void logout();
	void removeAccount();
};