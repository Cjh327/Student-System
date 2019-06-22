#pragma once
#ifndef __ADMIN_H__
#define __ADMIN_H__

#include "person.h"
#include "student.h"
#include "teacher.h"
#include <vector>
#include <string>

class Admin: public Person {
public:
    std::vector<Student*> studentList;
    std::vector<Teacher*> teacherList;
public:
    Admin(int, std::string, std::string, std::vector<Student*>, std::vector<Teacher*>);
    bool addStudent(int, std::string);
    bool addTeacher(int, std::string);
    bool deletePerson(int);
    int getStudentListSize() const;
    const Student* getStudentByIdx(int) const;
    int getTeacherListSize() const;
    const Teacher* getTeacherByIdx(int) const; 
};

#endif // !__ADMIN_H__
