#pragma once
#include <iostream>
#include <string.h>
#include <vector>
#include "Sha1.h"

#define LOGINLENGTH 10

class Chat{
public:
	Chat(){};
	
	/* методы чата для пользователя */
	void regUser(char _login[LOGINLENGTH], char _pass[], int pass_length); // добавление нового пользователя
	bool authUser(char _login[LOGINLENGTH], char _pass[], int pass_length); // авторизация
	void delUser(char _login[LOGINLENGTH]); // удаление пользователя
	void showUsers(); // вывод списка пользователей
	int getIndex(char _login[LOGINLENGTH]); // поиск индекса пользователя по имени
	bool logOut(char _login[LOGINLENGTH]); // выход
	void userTyping(char _from[LOGINLENGTH], char _to[LOGINLENGTH]); // написание сообщения
	
	/* сообщения */
	void getMsgs(char _from[LOGINLENGTH], char _to[LOGINLENGTH]); // вывод истории сообщений

private:
	enum userStatus{
		online,
		offline
	};

	// класс пользователя
	struct User{
		User() : login(""), pass_sha1_hash(0), name(""), age(-1), status(userStatus::offline){}
		~User(){}

		User(char _login[LOGINLENGTH], uint* sh1) : name(""), age(-1), status(userStatus::offline){
			strcpy(login, _login);
			pass_sha1_hash = sh1;
		}		
		
		/* методы пользователя */
		std::string getStatus();
		void switchStatus();

		// переменные пользователя
		char login[LOGINLENGTH];
		uint* pass_sha1_hash;

		std::string name;
		int age;
		userStatus status;
	};
	
	// класс сообщений
	struct Msg{ 
		Msg() : _sender(""), _recipient(""), _msg(""){}
		Msg(char _from[LOGINLENGTH], char _to[LOGINLENGTH], std::string msg) :  _msg(msg){
			strcpy(_sender, _from);
			strcpy(_recipient, _to);
		}
		~Msg(){}
		
		// переменные класса сообщений
		char _sender[LOGINLENGTH];
		char _recipient[LOGINLENGTH];
		std::string _msg;
	};

	// хранение списка пользователей и сообщений
	std::vector <User> _users{};
	std::vector <Msg> _messages{};
};
