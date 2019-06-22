#include "teacher.h"
#include <cassert>
using namespace std;

Teacher::Teacher(int _id, string _name, string _password, vector<Course*>& _courseList): Person(_id, _name, _password) {
    courseList = _courseList;
}

Teacher::Teacher(int _id, string _name, string _password): Person(_id, _name, _password) {}

Teacher::Teacher(int _id, string _name): Person(_id, _name, to_string(_id)) {}

bool Teacher::addCourse(string courseName, string courseTime, int credit, int capacity) {
    for (Course* pc : courseList) {
        if (pc->getName() == courseName) {
            return false;
        }
    }
    courseList.push_back(new Course(getId()*20 + (int)courseList.size(), courseName, credit, getId(), courseTime, capacity));
    return true;
}

bool Teacher::deleteCourse(int id) {
     for (auto iter = courseList.begin(); iter != courseList.end(); iter++) {
        if ((*iter)->getId() == id) {
            (*iter)->disable();
            courseList.erase(iter);
            return true;
        }
    }
    return false;
}

Course* Teacher::getCourse(int k) const {
    assert(k < (int)courseList.size());
    return courseList[k];
}

const int Teacher::getCourseNum() const {
    return (int)courseList.size();
}

Course* Teacher::getCourseById(int courseId) const {
    for (Course* p : courseList) {
        if (p->getId() == courseId) {
            return p;
        }
    }
    return nullptr;
}

void Teacher::deleteAllCourses() {
    for (Course* pc : courseList) {
        pc->disable();
    }
}