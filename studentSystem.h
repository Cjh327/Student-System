#pragma once
#ifndef __STUDENTSYSTEM_H__
#define __STUDENTSYSTEM_H__

#include "person.h"
#include "student.h"
#include "teacher.h"
#include "admin.h"
#include <locale.h>
#include <unistd.h>  
#include <errno.h>  
#include <fcntl.h>  
#include <cmath>
#include <unistd.h>
#include <iostream>
#include <ncurses.h>
#include <cstdlib>
#include <vector>
#include <map>

enum SysState { START, STUDENT, TEACHER, ADMIN };
enum Role { ROLE_DUMMY, ROLE_STUDENT, ROLE_TEACHER, ROLE_ADMIN };

class StudentSystem {
private:
	const int left = 10, right = 115, up = 5, down = 30;
	const int studentX = (left + right) / 4 - 4, intervalX = (left + right) / 4;
	const int opX0 = (left + right) / 6 - 4, intervalXOp0 = (left + right) / 6;
	const int opX1 = (left + right) / 5 - 4, intervalXOp1 = (left + right) / 5;
	int curY, curX;
	Role role;
	std::string stuFile, teaFile, admFile, crsFile;

	std::vector<Student*> studentList;
	std::vector<Teacher*> teacherList;
	std::vector<Admin*> adminList;
	std::vector<Course*> courseList;
	std::map<int, Course*> courseMap;
	Student* pStudent;
	Teacher* pTeacher;
	Admin* pAdmin;
	SysState state;
	void initScreen();
	/* Start Menu */
	void showStartMenu();
	/* Student Menu */
	void showStudentMenu(const Student*);
	void showStudentcheckCourseMenu(const Student*);
	void showStudentSelectedCourseMenu(const Student*);
	void showStudentAddCourseMenu(const Student*);
	void showStudentDropCourseMenu(const Student*);
	void showStudentcheckScoreMenu(const Student*);
	/* Teacher Menu */
	void showTeacherMenu(const Teacher*);
	void showTeacherAddCourseMenu(const Teacher*);
	void showTeacherModifyCourseMenu(const Teacher*);
	void showTeacherCheckCourseMenu(const Teacher*);
	void showTeacherCheckListMenu(const Teacher*);
	void showTeacherDeleteCourseMenu(const Teacher*);
	void showTeacherScoreMenu(const Teacher*);
	void showTeacherModifyScoreMenu(const Teacher*);
	/* Admin Menu */
	void showAdminMenu(const Admin*);
	void showAdminInsertStudentMenu(const Admin*);
	void showAdminInsertTeacherMenu(const Admin*);
	void showAdminCheckPersonMenu(const Admin*);
	void showAdminDeletePersonMenu(const Admin*);
public:
	StudentSystem(std::string, std::string, std::string, std::string);
	void run();
};

#endif // !__STUDENTSYSTEM_H__
