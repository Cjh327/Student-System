#include "studentSystem.h"
#include <iostream>
using namespace std;

int main() {
	StudentSystem sys("courses.txt", "students.txt", "teachers.txt", "admins.txt");
	sys.run();
	return 0;
}