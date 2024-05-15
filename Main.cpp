#include <iostream>
#include <cstring>
#include "Chat.h"
#include "Sha1.h"
#include "Func.h"

int main(){
	Chat chat;
	chat.readUser();
		
	std::cout << "---------- Добро пожаловать в чат ----------" << std::endl;
	std::cout << "Меню:\n";

	bool running = true;
	bool logged = false;
	
	while(running){
		std::string _login;
		char _pass[16];

		userHelp();
		std::cout << "Главное меню: ";
		
		char choice;
		std::cin >> choice;
		std::cin.ignore(256, '\n');
		
		switch(choice){
			case 'r':
				system("clear");
				std::cout << "Регистрация нового пользователя\n";
				std::cout << "Придумайте логин: ";
				std::getline(std::cin, _login, '\n');
				
				std::cout << "Придумайте пароль: ";
				std::cin.getline(_pass, 16, '\n');
				
				chat.regUser(_login, _pass, std::strlen(_pass));
				break;
			case 'l':
				system("clear");
				std::cout << "Авторизация пользователя\n";
				std::cout << "Введите логин: ";
				std::getline(std::cin, _login, '\n');
				
				std::cout << "Введите пароль: ";
				std::cin.getline(_pass, 16, '\n');

				logged = chat.authUser(_login, _pass, std::strlen(_pass));
				while(logged){
					chat.userRuntime(_login);
					logged = chat.logOut(_login);
				}
				break;
			case 'c':
				system("clear");
				cout << "Список пользователей:\n----------\nЛогин\tСтатус\n";
				chat.showUsers();
				cout << "----------\n\n";
				break;
			case 'q':
				running = false;
				break;
			default:
				system("clear");
				cout << "Неверное значение!\n";
				break;
		}
	}
	return 0;
}
