#include <iostream>
#include <string>
#include <ctime>
#include "Func.h"

void userHelp(){
	std::cout << "[r]egistration\tдля регистрации нового пользователя\n[l]og in\tдля входа в аккаунт\n[c]atalog\tдля вывода списка пользователей\n[q]uit\tдля выхода из программы\n" << std::endl;
}

std::string getTime(){
	std::time_t result = std::time(nullptr);
	char mbstr[100];
	std::strftime(mbstr, sizeof(mbstr), "%T", std::localtime(&result));
	return (std::string)mbstr;
}
