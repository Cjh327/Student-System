#pragma once
#ifndef __STUDENT_H__
#define __STUDENT_H__

#include "person.h"
#include "course.h"
#include <vector>

class Course;

class Student: public Person {
public:
	std::vector<Course*> courseList;
public:
	Student(const int, const std::string, const std::string, const std::vector<Course*>&);
	Student(const int, const std::string, const std::string);
	Student(const int, const std::string);
	std::vector<Course*> getCourseList() const;
	bool addCourse(Course* pc, int score = -1);
	bool dropCourse(int);
	void dropAllCourses();
	// TODO: 修改密码
};

#endif // !__STUDENT_H__
