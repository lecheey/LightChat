#include <iostream>
#include <fstream>
#include <filesystem>
#include <string>
#include <cstring>
#include <vector>
#include "Chat.h"

namespace fs = std::filesystem; 

// Пользователь
void Chat::regUser(std::string _login, char _pass[], int pass_length){
	std::cout << "Создание аккаунта пользователя...";

	for(int i = 0; i < _login.size(); i++){
		if(_login[i] == ';' || _login [i] == '/' || _login [i] == ':' ){
			std::cout << "Ошибка\nНедопустимые символы (; или /)\n" << std::endl;
			return;
		}
	}

	for(int i = 0; i < _users.size(); i++){
		if(_login == _users[i].login){
			std::cout << "Ошибка\nЛогин занят, выберите другой логин!\n" << std::endl;
			return;
		}
	}

	uint* h = sha1(_pass, pass_length);
	User newUser(_login, to_string(*h));
	_users.push_back(newUser);
	++_usercount;
	
	writeUser(_login, to_string(*h));
	std::cout << "Успешно\nНовый пользователь добавлен!\n";
	
	delete[] h;
}

bool Chat::authUser(std::string _login, char _pass[], int pass_length){
	std::cout << "Проверка данных пользователя...";
	int size = _users.size();
	int i = 0;
	while(_login != _users[i].login && i < size){
		i++;
	}
	if(i >= size){
		std::cout << "Ошибка\nНеверный логин!\n";
		return false;
	}

	uint* h = sha1(_pass, pass_length);
	bool result{_users[i].pass_sha1_hash == to_string(*h)};
	
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

bool Chat::logOut(std::string _login){
	_users[getIndex(_login)].switchStatus();
	_messages.clear();
	std::cout << "Пользователь " << _login <<  " вышел из чата!\n";
	return false; 
}

void Chat::delUser(std::string _login){
	std::cout << "Удаление данных пользователя...";
	if(_users.empty()){ std::cout << "Ошибка\nПользователь не найден!\n"; return; }

	for(int i = 0; i < _users.size(); i++){
		if(_login == _users[i].login){
			_users.erase(_users.begin() + i);
			std::cout << "Успешно\nДанные удалены!\n";
			return;
		}
	}

	std::cout << "Ошибка\nПользователь на найден!\n";
}

void Chat::writeUser(std::string _login, std::string _pass){
	fs::path filepath{"accounts.txt"};
	std::ofstream file(filepath, std::ios::out);
    if(file.is_open()){
		file << _usercount << '\n';
		for(int i = 0; i < _usercount; i++){
			file << _users[i].login << ':' << _users[i].pass_sha1_hash <<',';
		}
		file.close();
    }
}

void Chat::readUser(){
	fs::path filepath{"accounts.txt"};
	std::ifstream file(filepath, std::ios::in);
	
	std::string x, y, temp;
	if(file.is_open()){
		getline(file, temp, '\n');
		_usercount = stoi(temp);
		for(int i = 0; i < _usercount; i++){
			getline(file, x, ':');
			getline(file, y, ',');
			User user(x,y);
			_users.push_back(user);
		}	
		file.close();
	}
}

// Выполнение программы
void Chat::userRuntime(std::string _login){
	while(true){
		std::cout << "Введите имя собеседника ([q]uit для выхода):\n----------\nall\t(общий диалог)\n";
		showUsers();
		std::cout << "----------\n";

		std::cout << _login << ": ";
		std::string _to;
		std::getline(std::cin, _to, '\n');
		
		if(_to == "q"){
			return;
		}

		system("clear");	
		if(_to == "all"){
			std::cout << "---------- Групповой чат ----------" << std::endl;
		}
		else if(getIndex(_to) < 0){ continue; }
		else{
			std::cout << "---------- Собеседник  " << _to << " ----------" << std::endl; 
		}
		getMsgs(_login, _to);
		showMsgs(_login, _to);
		userTyping(_login, _to);	
	}	
}

void Chat::userTyping(std::string _from, std::string _to){
	while(true){
		std::string msg_body;
		std::cout << _from << ": ";
		std::getline(std::cin, msg_body, '\n');

		if(msg_body == "q"){
			system("clear");
			break;
		}
		else{
			saveMsg(_from, _to, msg_body);
		}
	}
}

// сообщения
void Chat::saveMsg(std::string _from, std::string _to, std::string _msg){
	fs::path dir("dialogs");
	if(!fs::exists(dir)){
		create_directory(dir);
	}

	if(_to != "all"){
		fs::path filepath1{dir / (_from + _to + ".txt")};
		fs::path filepath2{dir / (_to + _from + ".txt")};

		std::ofstream file1(filepath1, std::ios::app);
		if(file1.is_open()){
    	   file1 << getTime() << ';' << _from << ';' << _to << ';' << _msg <<'\n';
    	   file1.close();
		}
		
		std::ofstream file2(filepath2, std::ios::app);
		if(file2.is_open()){
    	   file2 << getTime() << ';' << _from << ';' << _to << ';' << _msg <<'\n';
    	   file2.close();
		}
	}
	else{
		fs::path filepath{dir / "group.txt"};
		std::ofstream file(filepath, std::ios::app);
    	if(file.is_open()){
    	   file << getTime() << ';' << _from << ';' << _to << ';' << _msg <<'\n';
    	   file.close();
    	}
	}
}

void Chat::getMsgs(std::string _from, std::string _to){
	fs::path dir("dialogs");
	if(fs::exists(dir)){
		std::string d, f, t, m, paths;

		paths = _to != "all" ?  (_from + _to + ".txt") : "group.txt";
		
		fs::path filepath{dir / paths};
		std::ifstream file(filepath, std::ios::in);
		
		if(file.is_open()){
			while(getline(file, d, ';')){
				getline(file, f, ';');
				getline(file, t, ';');
				getline(file, m, '\n');

				Msg msg(d, f, t,m);
				_messages.push_back(msg);
			}
			file.close();
		}
	}
}

void Chat::showMsgs(std::string _from, std::string _to){
	for(int i = 0; i < _messages.size(); i++){
		if(_to == "all" && _to == _messages[i]._receiver){
			std::cout << _messages[i]._sender << " (" << _messages[i]._time <<  "): " <<  _messages[i]._msg << std::endl;
		}
		else if(_from == _messages[i]._sender && _to == _messages[i]._receiver){
			std::cout << _messages[i]._sender << " (" << _messages[i]._time <<  "): " <<  _messages[i]._msg << std::endl;
		}
		else if(_from == _messages[i]._receiver && _to == _messages[i]._sender){
			std::cout << _messages[i]._sender << " (" << _messages[i]._time <<  "): " <<  _messages[i]._msg << std::endl;
		}
	}
}

// дополнительные функции
void Chat::showUsers(){
	for(int i = 0; i < _users.size(); i++){
		std::cout << _users[i].login << '\t' << _users[i].getStatus() << '\t' << '\n';
	}
}

int Chat::getIndex(std::string _login){
	for(int i = 0; i < _users.size(); i++){
		if(_login == _users[i].login){
			return i;
		}
	}
	std::cout << "Пользователь не найден!\n" << std::endl;
	return -1;
}

std::string Chat::User::getStatus(){
	return this->status == 0 ? "online" : "offline";
}

void Chat::User::switchStatus(){
	if(this->status == 1){
		this->status = userStatus::online;
	}
	else{
		this->status = userStatus::offline;
	}
}
