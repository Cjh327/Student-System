#pragma once
#ifndef __COURSE_H__
#define __COURSE_H__

#include "student.h"
#include <string>
#include <vector>

struct StudentInfo {
	int id;
	int score;
	StudentInfo(int _id, int _score = -1): id(_id), score(_score) {}
};

class Course {
private:
	bool exist;
	int id;
	std::string name;
	std::string secTime;
	int teacherId;
	int credit;
	int maxNum;
public:
	std::vector<StudentInfo> studentList;
public:
	Course(int, std::string, int, int, std::string, int);
	void printCourse();
	bool addStudent(int, int);
	bool eraseStudent(int);
	int getId() const;
	std::string getName() const;
	int getCredit() const;
	int getMaxNum() const;
	int getStuNum() const;
	int getTeacherId() const;
	std::string getSecTime() const;
	bool setStudentScore(int, int, int);
	void disable();
	bool resetCapacity(int);
	bool resetCredit(int);
	bool resetSecTime(std::string);
	bool resetName(std::string);
	int getScoreByStudentId(int);
};

#endif // !__COURSE_H__
