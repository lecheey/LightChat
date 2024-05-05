#include <iostream>
#include <string.h>
#include <vector>
#include "Chat.h"

/* Чат */
// добавление
void Chat::regUser(char _login[LOGINLENGTH], char _pass[], int pass_length){
	std::cout << "Создание аккаунта пользователя...";
	for(int i = 0; i < _users.size(); i++){
		if(!strcmp(_login, _users[i].login)){
			std::cout << "Ошибка\nЛогин занят, выберите другой логин!\n" << std::endl;
			return;
		}
	}

	uint* h = sha1(_pass, pass_length);
	User newUser(_login, h);
	_users.push_back(newUser);
	std::cout << "Успешно\nНовый пользователь добавлен!\n";
}

// авторизация
bool Chat::authUser(char _login[LOGINLENGTH], char _pass[], int pass_length){
	std::cout << "Проверка данных пользователя...";
	int size = _users.size();
	int i = 0;
	while(strcmp(_login, _users[i].login) && i < size){
		i++;
	}
	if(i >= size){
		std::cout << "Ошибка\nНеверный логин!\n";
		return false;
	}

	uint* h = sha1(_pass, pass_length);
	bool result = !memcmp(_users[i].pass_sha1_hash, h, SHA1HASHLENGTHBYTES);
	
	if(!result){
		std::cout << "Ошибка\nНеверный пароль!\n";
	}
	else{
		_users[i].switchStatus();
		std::cout << "Успешно\nДобро пожаловать!\n";
	}

	delete[] h;
	return result;
}

// выход
bool Chat::logOut(char _login[LOGINLENGTH]){
	_users[getIndex(_login)].switchStatus();
	std::cout << "Пользователь вышел из чата!\n";
	return false; 
}

// удаление
void Chat::delUser(char _login[LOGINLENGTH]){
	std::cout << "Удаление данных пользователя...";
	if(_users.empty()){ std::cout << "Ошибка\nПользователь не найден!\n"; return; }

	for(int i = 0; i < _users.size(); i++){
		if(strcmp(_login, _users[i].login)){
			_users.erase(_users.begin() + i);
			std::cout << "Успешно\nДанные удалены!\n";
			return;
		}
	}

	std::cout << "Ошибка\nПользователь на найден!\n";
}

// функция написания сообщения
void Chat::userTyping(char _from[LOGINLENGTH], char _to[LOGINLENGTH]){
	while(true){
		std::string msg_body;
		std::cout << _from << ": ";
		std::getline(std::cin, msg_body);

		if(msg_body == "q"){
			system("clear");
			break;
		}
		else{
			_messages.emplace_back(_from, _to, msg_body);
		}
	}
}

// вывод списка пользователей
void Chat::showUsers(){
	for(int i = 0; i < _users.size(); i++){
		std::cout << _users[i].login << '\t' << _users[i].name << '\t' << _users[i].getStatus() << '\n';
	}
}

// поиск индекса (проверка существования пользователя) по логину
int Chat::getIndex(char _login[LOGINLENGTH]){ // поиск индекса пользователя по имени
	for(int i = 0; i < _users.size(); i++){
		if(!strcmp(_login, _users[i].login)){
			return i;
		}
	}
	std::cout << "Пользователь не найден!\n" << std::endl;
	return -1;
}

// вывод истории сообщений
void Chat::getMsgs(char _from[LOGINLENGTH], char _to[LOGINLENGTH]){ // вывод истории сообщений
	for(int i = 0; i < _messages.size(); i++){ // групповые сообщения
		if(!strcmp(_to, "all") && !strcmp(_to, _messages[i]._recipient)){
			std::cout << _messages[i]._sender << ": " <<  _messages[i]._msg << std::endl;
		}
		else if(!strcmp(_from, _messages[i]._sender) && !strcmp(_to, _messages[i]._recipient)){
			std::cout << _messages[i]._sender << ": " <<  _messages[i]._msg << std::endl;
		}
		else if(!strcmp(_from, _messages[i]._recipient) && !strcmp(_to, _messages[i]._sender)){
			std::cout << _messages[i]._sender << ": " <<  _messages[i]._msg << std::endl;
		}
	}
}

/* пользователь*/
// пролучение данных статуса
std::string Chat::User::getStatus(){
	if(this->status == 0){
		return "online";
	}
	else{
		return "offline";
	}
}

// изменение статуса
void Chat::User::switchStatus(){
	if(this->status == 1){
		this->status = userStatus::online;
	}
	else{
		this->status = userStatus::offline;
	}
}
