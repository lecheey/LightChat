#include <iostream>
#include <string>
#include "Chat.h"
#include "Sha1.h"
using namespace std;

void userHelp(){
	cout << "[r]egistration\tдля регистрации нового пользователя\n[l]og in\tдля входа в аккаунт\n[c]atalog\tдля вывода списка пользователей\n[q]uit\tдля выхода из программы\n" << endl;
}

int main(){
	Chat chat;
		
	cout << "---------- Добро пожаловать в чат ----------" << endl;
	cout << "Для работы с программой используйте:\n";

	bool running = true;
	while(running){
		// переменные для логина и пароля
		std::string login, pass;
		char* _l;
		char* _p;
	
		userHelp();
		cout << "Главное меню: ";
		char choice;
		cin >> choice; // выбор действий пользователя
		cin.ignore(256, '\n'); // игнорирование ввода символов после первого
		
		// опции
		switch(choice){
			case 'r':
				system("clear");
				std::cout << "Регистрация нового пользователя\n";
				std::cout << "Придумайте логин: ";
				std::getline(std::cin, login);
				_l = login.data();
				
				std::cout << "Придумайте пароль: ";
				std::getline(std::cin, pass);
				_p = pass.data();
				
				chat.regUser(_l, _p, size(pass));
				break;
			case 'l':
				system("clear");
				std::cout << "Авторизация пользователя\n";
				std::cout << "Введите логин: ";
				std::getline(std::cin, login);
				_l = login.data();
				
				std::cout << "Введите пароль: ";
				std::getline(std::cin, pass);
				_p = pass.data();

				bool logged;
				logged = chat.authUser(_l, _p, size(pass));
				
				// отправка сообщений
				while(logged){
					// выбор собеседника (чата)	
					std::cout << "Введите имя собеседника ([q]uit для выхода):\n----------\nall\t(общий диалог)\n";
					chat.showUsers();
					
					std::cout << login << ": "; // имя отправителя
					std::string to;
					std::getline(std::cin, to);
					
					// отмена
					if(to == "q"){
						logged = chat.logOut(_l);
						break;
					}

					char* _to;
					_to = to.data();

					system("clear");
					if(!strcmp(_to, "all")){
						std::cout << "---------- Групповой чат ----------" << std::endl;
					}
					else if(chat.getIndex(_to) < 0){ continue; }
					else{
						std::cout << "---------- Собеседник  " << to << " ----------" << std::endl; 
					}
					chat.getMsgs(_l, _to); // история сообщений
					chat.userTyping(_l, _to); // отправка сообщений
				}	
				break;
			case 'c':
				system("clear");
				cout << "Список пользователей:\n----------\nлогин\tимя\tстатус\n";
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
