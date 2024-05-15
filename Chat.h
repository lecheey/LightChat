#pragma once
#include <iostream>
#include <string>
#include <cstring>
#include <vector>
#include "Sha1.h"
#include "Func.h"

class Chat{
public:
	Chat(){};
	
	void regUser(std::string _login, char _pass[], int pass_length);
	bool authUser(std::string _login, char _pass[], int pass_length);
	bool logOut(std::string _login);
	void delUser(std::string _login);	
	
	void writeUser(std::string _login, std::string _pass);
	void readUser();

	void userRuntime(std::string _login);
	void userTyping(std::string _from, std::string _to);
	
	void saveMsg(std::string _from, std::string _to, std::string _msg);
	void getMsgs(std::string _from, std::string _to);
	void showMsgs(std::string _from, std::string _to);
	
	int getIndex(std::string _login);
	void showUsers();
private:
	enum userStatus{
		online,
		offline
	};

	struct User{
		User() : login(""), pass_sha1_hash(""), status(userStatus::offline){}
		User(std::string _login, std::string sh1) :
					login(_login), pass_sha1_hash(sh1), status(userStatus::offline){}		
		~User(){}
		
		std::string getStatus();
		void switchStatus();

		std::string login;
		std::string pass_sha1_hash;

		userStatus status;
	};
	
	struct Msg{
		Msg() : _sender(""), _receiver(""), _msg(""){}
		Msg(std::string time, std::string _from, std::string _to, std::string msg) :
					_time(time), _msg(msg), _sender(_from), _receiver(_to){}
		~Msg(){}
		
		std::string _sender;
		std::string _receiver;
		std::string _msg;
		std::string _time;
	};

	std::vector <User> _users{};
	std::vector <Msg> _messages{};
	int _usercount{0};
};
