#include "admin.h"
#include <string>
using namespace std;

Admin::Admin(int _id, string _name, string _password, vector<Student*> _studentList, vector<Teacher*> _teacherList): Person(_id, _name, _password) {
    studentList = _studentList;
    teacherList = _teacherList;
}

bool Admin::addStudent(int id, string name) {
    for (Student* pst : studentList) {
        if (pst->getId() == id) {
            return false;
        }
    }
    studentList.push_back(new Student(id, name));
    return true;
}

bool Admin::addTeacher(int id, string name) {
    for (Teacher* pt : teacherList) {
        if (pt->getId() == id) {
            return false;
        }
    }
    teacherList.push_back(new Teacher(id, name));
    return true;
}

int Admin::getStudentListSize() const {
    return (int)studentList.size();
}

const Student* Admin::getStudentByIdx(int idx) const {
    if (idx >= (int)studentList.size()) {
        return nullptr;
    }
    return studentList[idx];
}

int Admin::getTeacherListSize() const {
    return (int)teacherList.size();
}

const Teacher* Admin::getTeacherByIdx(int idx) const {
    if (idx >= (int)teacherList.size()) {
        return nullptr;
    }
    return teacherList[idx];
}

bool Admin::deletePerson(int id) {
    for (auto iter = studentList.begin(); iter != studentList.end(); iter++) {
        if ((*iter)->getId() == id) {
            (*iter)->dropAllCourses();
            studentList.erase(iter);
            return true;
        }
    }
    for (auto iter = teacherList.begin(); iter != teacherList.end(); iter++) {
        if ((*iter)->getId() == id) {
            (*iter)->deleteAllCourses();
            teacherList.erase(iter);
            return true;
        }
    }
    return false;
}