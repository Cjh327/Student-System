#include "course.h"
#include <string>
#include <vector>
#include <iostream>
#include <cassert>
using namespace std;

Course::Course(int _id, string _name, int _credit, int _teacherId, string _time, int _maxnum) {
	id = _id;
    name = _name;
    credit = _credit;
    teacherId = _teacherId;
    secTime = _time;
    maxNum = _maxnum;
    exist = true;
}

void Course::printCourse() {
    cout << "id:" << id << " name:" << name << " credit:" << credit << 
    " teacherId:" << teacherId << " secTime:" << secTime << " maxNum:" << maxNum << endl;
}

bool Course::addStudent(int studentId, int score) {
    for (auto iter = studentList.begin(); iter != studentList.end(); iter++) {
        if ((*iter).id == studentId) {
            return false;
        }
    }
    studentList.push_back(StudentInfo(studentId, score));
    return true;
}

bool Course::eraseStudent(int studentId) {
    for (auto iter = studentList.begin(); iter != studentList.end(); iter++) {
        if ((*iter).id == studentId) {
            studentList.erase(iter);
            return true;
        }
    }
    return false;
}

int Course::getId() const {
    return id;
}

string Course::getName() const {
    return name;
}

int Course::getCredit() const {
    return credit;
}

int Course::getMaxNum() const {
    return maxNum;
}

int Course::getStuNum() const {
    return (int)studentList.size();
}

int  Course::getTeacherId() const {
    return teacherId;
}

string  Course::getSecTime() const {
    return secTime;
}

bool Course::setStudentScore(int stidx, int stid, int sc) {
    assert(stidx < (int)studentList.size() and studentList[stidx].id == stid);
    studentList[stidx].score = sc;
    return true;
}

void Course::disable() {
    exist = false;
}

bool Course::resetCapacity(int _capacity) {
    if (_capacity >= (int)studentList.size()) {
        maxNum = _capacity;
        return true;
    } else {
        return false;
    }
}

bool Course::resetCredit(int _credit) {
    credit = _credit;
}

bool Course::resetSecTime(string _secTime) {
    secTime = _secTime;
}

bool Course::resetName(string _name) {
    name = _name;
}

int Course::getScoreByStudentId(int stuId) {
    for (StudentInfo si : studentList) {
        if (si.id == stuId) {
            return si.score;
        }
    }
    assert(false);
    return -2;
}