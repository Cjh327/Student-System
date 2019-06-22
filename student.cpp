#include "student.h"
#include <string>
#include <cassert>
#include <iostream>
using namespace std;
/*
Student::Student(const int _id, const string _name, const string _password): Person(_id, _name, _password) {
    for (Course* c : _courseList) {
        addCourse(c);
    }
}
*/


Student::Student(const int _id, const string _name, const string _password): Person(_id, _name, _password) {}

Student::Student(const int _id, const string _name): Person(_id, _name, to_string(_id)) {}

vector<Course*> Student::getCourseList() const {
    return courseList;
}

bool Student::addCourse(Course* course, int score) {
    if (course->getStuNum() >= course->getMaxNum()) {
        return false;
    }
    for (const Course* c : courseList) {
        if (c->getId() == course->getId()) {
            return false;
        }
    }
    if (not course->addStudent(getId(), score)) {
        assert(false);
    }
    courseList.push_back(course);
    return true;
}

bool Student::dropCourse(int courseNo) {
    for (auto iter = courseList.begin(); iter != courseList.end(); iter++) {
        if ((*iter)->getId() == courseNo) {
            if (not (*iter)->eraseStudent(getId())) {
                assert(false);
            }
            courseList.erase(iter);
            return true;
        }
    }
    return false;
}

void Student::dropAllCourses() {
    for (auto iter = courseList.begin(); iter != courseList.end(); iter++) {
        if (not (*iter)->eraseStudent(getId())) {
            assert(false);
        }
    }
    courseList.clear();
}