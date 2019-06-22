#pragma once
#ifndef __TEACHER_H__
#define __TEACHER_H__

#include "person.h"
#include "course.h"
#include <vector>
#include <string>

class Teacher: public Person {
public:
    std::vector<Course*> courseList;
public:
    Teacher(int, std::string, std::string, std::vector<Course*>&);
    Teacher(int, std::string, std::string);
    Teacher(int, std::string);
    bool addCourse(std::string, std::string, int, int);
    bool deleteCourse(int);
    void deleteAllCourses();
    Course* getCourse(int) const;
    const int getCourseNum() const;
    Course* getCourseById(int) const;
};

#endif // !__TEACHER_H__
