#include "person.h"
#include <string>
using namespace std;

Person::Person(int _id, string _name, string _password) {
    id = _id;
    name = _name;
    password = _password;
}

int Person::getId() const {
    return id;
}

string Person::getName() const {
    return name;
}

bool Person::checkPassword(const string& str) const {
    return (password == str);
}

string Person::getPassword() const {
    return password;
}