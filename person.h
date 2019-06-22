#pragma once
#ifndef __PERSON_H__
#define __PERSON_H__

#include <string>

class Person {
private:
	int id;
	std::string name;
	std::string password;
public:
	Person(int, std::string, std::string);
	int getId() const;
	std::string getName() const;
	bool checkPassword(const std::string&) const;
	void login();
	void logout();
	std::string getPassword() const;
	bool modifyPassword();
};

#endif // !__PERSON_H__
