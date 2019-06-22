#include "studentSystem.h"
#include <iostream>
#include <string>
#include <cstring>
#include <cassert>
#include <fstream>
using namespace std;

vector<int> splitToInt(const string &s, const string &seperator){
    vector<int> result;
    typedef string::size_type string_size;
    string_size i = 0;
    while(i != s.size()){
        //找到字符串中首个不等于分隔符的字母
        int flag = 0;
        while(i != s.size() && flag == 0){
            flag = 1;
            for(string_size x = 0; x < seperator.size(); ++x) {
                if(s[i] == seperator[x]) {
                    ++i;
                    flag = 0;
                    break;
                }
            }
        }    
        //找到又一个分隔符，将两个分隔符之间的字符串取出；
        flag = 0;
        string_size j = i;
        while(j != s.size() && flag == 0){
            for(string_size x = 0; x < seperator.size(); ++x) {
                if(s[j] == seperator[x]){
                    flag = 1;
                    break;
                    }
            }
            if(flag == 0) ++j;
        }
        if(i != j) {
            result.push_back(stoi(s.substr(i, j-i)));
            i = j;
        }
    }
    return result;
}

vector<string> split(const string &s, const string &seperator){
    vector<string> result;
    typedef string::size_type string_size;
    string_size i = 0;
    while(i != s.size()){
        //找到字符串中首个不等于分隔符的字母
        int flag = 0;
        while(i != s.size() && flag == 0){
            flag = 1;
            for(string_size x = 0; x < seperator.size(); ++x) {
                if(s[i] == seperator[x]) {
                    ++i;
                    flag = 0;
                    break;
                }
            }
        }    
        //找到又一个分隔符，将两个分隔符之间的字符串取出；
        flag = 0;
        string_size j = i;
        while(j != s.size() && flag == 0){
            for(string_size x = 0; x < seperator.size(); ++x) {
                if(s[j] == seperator[x]){
                    flag = 1;
                    break;
                    }
            }
            if(flag == 0) ++j;
        }
        if(i != j) {
            result.push_back(s.substr(i, j-i));
            i = j;
        }
    }
    return result;
}

StudentSystem::StudentSystem(string courseFile, string studentFile, string teacherFile, string adminFile) {
    pStudent = nullptr;
    pTeacher = nullptr;
    pAdmin = nullptr;
    role = ROLE_DUMMY;
    stuFile = studentFile, teaFile = teacherFile, admFile = adminFile, crsFile = courseFile;
    ifstream courseStream, studentStream, teacherStream, adminStream;
    string str;
    /* load courses */
    courseStream.open(courseFile, ios::in);
    if (courseStream.fail()) {
        cout << "Fail to open course file!" << endl;
        assert(false);
    }
    while (getline(courseStream, str)) {
        vector<string> v = split(str, ",");
        Course *pCourse = new Course(stoi(v[0]), v[1], stoi(v[2]), stoi(v[3]), v[4], stoi(v[5]));
        courseList.push_back(pCourse);
        courseMap.insert(pair<int, Course*>(pCourse->getId(), pCourse));
    }
    courseStream.close();
    /* load students */
    studentStream.open(studentFile, ios::in);
    if (studentStream.fail()) {
        cout << "Fail to open student file!" << endl;
        assert(false);
    }
    while (getline(studentStream, str)) {
        vector<string> v = split(str, ",");
        if (v[3] == "-") {
            Student *pst = new Student(stoi(v[0]), v[1], v[2]);
            studentList.push_back(pst);
        } else {
            Student *pst = new Student(stoi(v[0]), v[1], v[2]);
            vector<string> v1 = split(v[3], "|");
            for (string s : v1) {
                vector<int> v2 = splitToInt(s, ":");
                assert(v2.size() == 2);
                auto it = courseMap.find(v2[0]);
                if (it != courseMap.end()) {   
                    pst->addCourse(it->second, v2[1]);
                }
            }
            studentList.push_back(pst);
        }
    }
    studentStream.close();
    /* load teachers */
    teacherStream.open(teacherFile, ios::in);
    if (teacherStream.fail()) {
        cout << "Fail to open teacher file!" << endl;
        assert(false);
    }
    while (getline(teacherStream, str)) {
        vector<string> v = split(str, ",");
        if (v[3] == "-") {
            Teacher *pt = new Teacher(stoi(v[0]), v[1], v[2]);
            teacherList.push_back(pt);
        } else {
            vector<int> v1 = splitToInt(v[3], "|");
            vector<Course*> cList;
            for (int courseNo : v1) {
                auto it = courseMap.find(courseNo);
                if (it == courseMap.end()) {
                    assert(false);
                }
                cList.push_back(it->second);
            }
            Teacher *pt = new Teacher(stoi(v[0]), v[1], v[2], cList);
            teacherList.push_back(pt);
        }
    }
    teacherStream.close();
    /* load admins */
    adminStream.open(adminFile, ios::in);
    if (adminStream.fail()) {
        cout << "Fail to open admin file!" << endl;
        assert(false);
    }
    while (getline(adminStream, str)) {
        vector<string> v = split(str, ",");
        Admin* pa = new Admin(stoi(v[0]), v[1], v[2], studentList, teacherList);
        adminList.push_back(pa);
    }
    adminStream.close();    
}

void StudentSystem::showStartMenu() {
    clear();
    int width = right - left;
    string str1(width, '*');
	mvaddstr(up, left, str1.data());
    string str2(width, ' ');
    str2[0] = '*'; str2[width - 1] = '*';
    for (int row = up + 1; row < down; row++) {
        mvaddstr(row, left, str2.data());
    }
	mvaddstr(down, left, str1.data());
    mvaddstr(up + 3, (left + right) / 2 - 13, "欢迎来到学生信息管理系统！");
    mvaddstr(up + 7, (left + right) / 2 - 8, "选择您的角色");
    mvaddstr(up + 10, studentX, "学生");
    mvaddstr(up + 10, studentX + intervalX, "教师");
    mvaddstr(up + 10, studentX + 2 * intervalX, "管理员");
    mvaddstr(up + 14, (left + right) / 2 - 10, "请输入账号密码");
    mvaddstr(up + 17, (left + right) / 2 - 10, "账号:");
    mvaddstr(up + 19, (left + right) / 2 - 10, "密码:");
    mvaddstr(down - 1, (left + right) / 2 - 6, "(按q退出)");
    curY = up + 10, curX = studentX;
    move(curY, curX);
    refresh();
}

void StudentSystem::showStudentMenu(const Student* p) {
    clear();
    int width = right - left;
    string str1(width, '*');
	mvaddstr(up, left, str1.data());
    string str2(width, ' ');
    str2[0] = '*'; str2[width - 1] = '*';
    for (int row = up + 1; row < down; row++) {
        mvaddstr(row, left, str2.data());
    }
	mvaddstr(down, left, str1.data());
    string stuStr = "学生: " + p->getName();
    mvaddstr(up + 3, (left + right) / 2 - (int)stuStr.size() / 2, stuStr.data());
    mvaddstr(up + 10, (left + right) / 2 - 8, "选择您的操作");
    mvaddstr(up + 19, opX0, "查看选课表");
    mvaddstr(up + 19, opX0 + intervalXOp0, "查看已选课程");
    mvaddstr(up + 19, opX0 + 2 * intervalXOp0, "选课");
    mvaddstr(up + 19, opX0 + 3 * intervalXOp0, "退课");
    mvaddstr(up + 19, opX0 + 4 * intervalXOp0, "查询成绩");
    move(curY, curX);
    refresh();
}

void StudentSystem::showStudentcheckCourseMenu(const Student* pst) {
    clear();
    int width = right - left;
    string str1(width, '*');
	mvaddstr(up, left, str1.data());
    string str2(width, ' ');
    str2[0] = '*'; str2[width - 1] = '*';
    for (int row = up + 1; row < down; row++) {
        mvaddstr(row, left, str2.data());
    }
	mvaddstr(down, left, str1.data());
    string titleStr("查看选课表");
    mvaddstr(up + 3, (left + right) / 2 - (int)titleStr.size() / 2, titleStr.data());
    int row = up + 5;
    mvaddstr(row, (left + right) / 8, "编号");
    mvaddstr(row, 2*(left + right) / 8, "课程名");
    mvaddstr(row, 3*(left + right) / 8, "学分");
    mvaddstr(row, 4*(left + right) / 8, "任课老师");
    mvaddstr(row, 5*(left + right) / 8, "上课时间");
    mvaddstr(row, 6*(left + right) / 8, "课程容量");
    for (const Course* pCourse : courseList) {
    //for (const Course* pCourse : pst->getCourseList()) {
        ++row;
        mvprintw(row, (left + right) / 8, "%d", pCourse->getId());
        mvaddstr(row, 2*(left + right) / 8, pCourse->getName().data());
        mvprintw(row, 3*(left + right) / 8, "%d", pCourse->getCredit());
        string teacherName;
        for (Teacher* teacher : teacherList) {
            if (pCourse->getTeacherId() == teacher->getId()) {
                teacherName = teacher->getName();
                break;
            }
        }
        mvaddstr(row, 4*(left + right) / 8, teacherName.data());
        mvaddstr(row, 5*(left + right) / 8, pCourse->getSecTime().data());
        string tmp = to_string(pCourse->getStuNum()) + "/" + to_string(pCourse->getMaxNum());
        mvaddstr(row, 6*(left + right) / 8, tmp.data());
    }
    refresh();
}

void StudentSystem::showStudentSelectedCourseMenu(const Student* pst) {
    clear();
    int width = right - left;
    string str1(width, '*');
	mvaddstr(up, left, str1.data());
    string str2(width, ' ');
    str2[0] = '*'; str2[width - 1] = '*';
    for (int row = up + 1; row < down; row++) {
        mvaddstr(row, left, str2.data());
    }
	mvaddstr(down, left, str1.data());
    string titleStr("查看已选课程");
    mvaddstr(up + 3, (left + right) / 2 - (int)titleStr.size() / 2, titleStr.data());
    int row = up + 5;
    mvaddstr(row, (left + right) / 8, "编号");
    mvaddstr(row, 2*(left + right) / 8, "课程名");
    mvaddstr(row, 3*(left + right) / 8, "学分");
    mvaddstr(row, 4*(left + right) / 8, "任课老师");
    mvaddstr(row, 5*(left + right) / 8, "上课时间");
    mvaddstr(row, 6*(left + right) / 8, "课程容量");
    for (const Course* pCourse : pst->getCourseList()) {
        ++row;
        mvprintw(row, (left + right) / 8, "%d", pCourse->getId());
        mvaddstr(row, 2*(left + right) / 8, pCourse->getName().data());
        mvprintw(row, 3*(left + right) / 8, "%d", pCourse->getCredit());
        string teacherName;
        for (Teacher* teacher : teacherList) {
            if (pCourse->getTeacherId() == teacher->getId()) {
                teacherName = teacher->getName();
                break;
            }
        }
        mvaddstr(row, 4*(left + right) / 8, teacherName.data());
        mvaddstr(row, 5*(left + right) / 8, pCourse->getSecTime().data());
        string tmp = to_string(pCourse->getStuNum()) + "/" + to_string(pCourse->getMaxNum());
        mvaddstr(row, 6*(left + right) / 8, tmp.data());
    }
    refresh();
}

void StudentSystem::showStudentAddCourseMenu(const Student* pst) {
    clear();
    int width = right - left;
    string str1(width, '*');
	mvaddstr(up, left, str1.data());
    string str2(width, ' ');
    str2[0] = '*'; str2[width - 1] = '*';
    for (int row = up + 1; row < down; row++) {
        mvaddstr(row, left, str2.data());
    }
	mvaddstr(down, left, str1.data());
    string titleStr("选课");
    mvaddstr(up + 3, (left + right) / 2 - (int)titleStr.size() / 2, titleStr.data());
    int row = up + 5;
    mvaddstr(row, (left + right) / 8, "编号");
    mvaddstr(row, 2*(left + right) / 8, "课程名");
    mvaddstr(row, 3*(left + right) / 8, "学分");
    mvaddstr(row, 4*(left + right) / 8, "任课老师");
    mvaddstr(row, 5*(left + right) / 8, "上课时间");
    mvaddstr(row, 6*(left + right) / 8, "课程容量");
    for (const Course* pCourse : courseList) {
        ++row;
        mvprintw(row, (left + right) / 8, "%d", pCourse->getId());
        mvaddstr(row, 2*(left + right) / 8, pCourse->getName().data());
        mvprintw(row, 3*(left + right) / 8, "%d", pCourse->getCredit());
        string teacherName;
        for (Teacher* teacher : teacherList) {
            if (pCourse->getTeacherId() == teacher->getId()) {
                teacherName = teacher->getName();
                break;
            }
        }
        mvaddstr(row, 4*(left + right) / 8, teacherName.data());
        mvaddstr(row, 5*(left + right) / 8, pCourse->getSecTime().data());
        string tmp = to_string(pCourse->getStuNum()) + "/" + to_string(pCourse->getMaxNum());
        mvaddstr(row, 6*(left + right) / 8, tmp.data());
    }
    curY = up + 6, curX = (left + right) / 8;
    move(curY, curX);
    refresh();
}

void StudentSystem::showStudentDropCourseMenu(const Student* pst) {
    clear();
    int width = right - left;
    string str1(width, '*');
	mvaddstr(up, left, str1.data());
    string str2(width, ' ');
    str2[0] = '*'; str2[width - 1] = '*';
    for (int row = up + 1; row < down; row++) {
        mvaddstr(row, left, str2.data());
    }
	mvaddstr(down, left, str1.data());
    string titleStr("查看已选课程");
    mvaddstr(up + 3, (left + right) / 2 - (int)titleStr.size() / 2, titleStr.data());
    int row = up + 5;
    mvaddstr(row, (left + right) / 8, "编号");
    mvaddstr(row, 2*(left + right) / 8, "课程名");
    mvaddstr(row, 3*(left + right) / 8, "学分");
    mvaddstr(row, 4*(left + right) / 8, "任课老师");
    mvaddstr(row, 5*(left + right) / 8, "上课时间");
    mvaddstr(row, 6*(left + right) / 8, "课程容量");
    for (const Course* pCourse : pst->getCourseList()) {
        ++row;
        mvprintw(row, (left + right) / 8, "%d", pCourse->getId());
        mvaddstr(row, 2*(left + right) / 8, pCourse->getName().data());
        mvprintw(row, 3*(left + right) / 8, "%d", pCourse->getCredit());
        string teacherName;
        for (Teacher* teacher : teacherList) {
            if (pCourse->getTeacherId() == teacher->getId()) {
                teacherName = teacher->getName();
                break;
            }
        }
        mvaddstr(row, 4*(left + right) / 8, teacherName.data());
        mvaddstr(row, 5*(left + right) / 8, pCourse->getSecTime().data());
        string tmp = to_string(pCourse->getStuNum()) + "/" + to_string(pCourse->getMaxNum());
        mvaddstr(row, 6*(left + right) / 8, tmp.data());
    }
    curY = up + 6, curX = (left + right) / 8;
    move(curY, curX);
    refresh();
}

void StudentSystem::showStudentcheckScoreMenu(const Student* pst) {
    clear();
    int width = right - left;
    string str1(width, '*');
	mvaddstr(up, left, str1.data());
    string str2(width, ' ');
    str2[0] = '*'; str2[width - 1] = '*';
    for (int row = up + 1; row < down; row++) {
        mvaddstr(row, left, str2.data());
    }
	mvaddstr(down, left, str1.data());
    string titleStr = "查询成绩";
    mvaddstr(up + 3, (left + right) / 2 - (int)titleStr.size() / 2, titleStr.data());
    mvaddstr(up + 6, (left + right) / 2 - 24, "课程编号");
    mvaddstr(up + 6, (left + right) / 2 - 8, "课程名");
    mvaddstr(up + 6, (left + right) / 2 + 16, "成绩");
    for (int i = 0; i < (int)pst->courseList.size(); i++) {
        Course* pc = pst->courseList[i];
        mvprintw(up + 7 + i, (left + right) / 2 - 24, "%d", pc->getId());
        mvaddstr(up + 7 + i, (left + right) / 2 - 8, pc->getName().c_str());
        int score = pc->getScoreByStudentId(pst->getId());
        if (score == -1) {
            mvaddstr(up + 7 + i, (left + right) / 2 + 16, "未录入");
        } else {
            mvprintw(up + 7 + i, (left + right) / 2 + 16, "%d", score);
        }
    }
    refresh();
}

void StudentSystem::showTeacherMenu(const Teacher* p) {
    clear();
    int width = right - left;
    string str1(width, '*');
	mvaddstr(up, left, str1.data());
    string str2(width, ' ');
    str2[0] = '*'; str2[width - 1] = '*';
    for (int row = up + 1; row < down; row++) {
        mvaddstr(row, left, str2.data());
    }
	mvaddstr(down, left, str1.data());
    string stuStr = "教师: " + p->getName();
    mvaddstr(up + 3, (left + right) / 2 - (int)stuStr.size() / 2, stuStr.data());
    mvaddstr(up + 10, (left + right) / 2 - 8, "选择您的操作");
    mvaddstr(up + 16, opX1, "发布课程");
    mvaddstr(up + 16, opX1 + intervalXOp1, "查看课程");
    mvaddstr(up + 16, opX1 + 2 * intervalXOp1, "修改课程");
    mvaddstr(up + 16, opX1 + 3 * intervalXOp1, "删除课程");
    mvaddstr(up + 19, opX1, "查询名单");
    mvaddstr(up + 19, opX1 + intervalXOp1, "录入成绩");
    mvaddstr(up + 19, opX1 + 2 * intervalXOp1, "修改成绩");
    move(curY, curX);
    refresh();
}

void StudentSystem::showTeacherAddCourseMenu(const Teacher* p) {
    clear();
    int width = right - left;
    string str1(width, '*');
	mvaddstr(up, left, str1.data());
    string str2(width, ' ');
    str2[0] = '*'; str2[width - 1] = '*';
    for (int row = up + 1; row < down; row++) {
        mvaddstr(row, left, str2.data());
    }
	mvaddstr(down, left, str1.data());
    string titleStr = "发布课程";
    mvaddstr(up + 3, (left + right) / 2 - (int)titleStr.size() / 2, titleStr.data());
    mvaddstr(up + 5, (left + right) / 2 - 8, "请输入课程信息");
    mvaddstr(up + 7, (left + right) / 2 - 16, "课程名:");
    mvaddstr(up + 8, (left + right) / 2 - 16, "上课时间:");
    mvaddstr(up + 9, (left + right) / 2 - 16, "学分:");
    mvaddstr(up + 10, (left + right) / 2 - 16, "课程容量:");
    refresh();
}


void StudentSystem::showTeacherModifyCourseMenu(const Teacher* p) {
    clear();
    int width = right - left;
    string str1(width, '*');
	mvaddstr(up, left, str1.data());
    string str2(width, ' ');
    str2[0] = '*'; str2[width - 1] = '*';
    for (int row = up + 1; row < down; row++) {
        mvaddstr(row, left, str2.data());
    }
	mvaddstr(down, left, str1.data());
    string titleStr = "修改课程";
    mvaddstr(up + 3, (left + right) / 2 - (int)titleStr.size() / 2, titleStr.data());
    mvaddstr(up + 5, (left + right) / 2 - 16, "请输入课程编号:");
    refresh();
}

void StudentSystem::showTeacherCheckCourseMenu(const Teacher* p) {
    clear();
    int width = right - left;
    string str1(width, '*');
	mvaddstr(up, left, str1.data());
    string str2(width, ' ');
    str2[0] = '*'; str2[width - 1] = '*';
    for (int row = up + 1; row < down; row++) {
        mvaddstr(row, left, str2.data());
    }
	mvaddstr(down, left, str1.data());
    string stuStr = "教师: " + p->getName();
    mvaddstr(up + 3, (left + right) / 2 - (int)stuStr.size() / 2, stuStr.data());
    int row = up + 5;
    mvaddstr(row, (left + right) / 8, "编号");
    mvaddstr(row, 2*(left + right) / 8, "课程名");
    mvaddstr(row, 3*(left + right) / 8, "学分");
    mvaddstr(row, 4*(left + right) / 8, "任课老师");
    mvaddstr(row, 5*(left + right) / 8, "上课时间");
    mvaddstr(row, 6*(left + right) / 8, "课程容量");
    for (auto pCourse : p->courseList) {
//    for (int i = 0; i < p->getCourseNum(); i++) {
  //      Course* pCourse = p->getCourse(i);
        if (pCourse == nullptr) {
            assert(false);
        }
        ++row;
        mvprintw(row, (left + right) / 8, "%d", pCourse->getId());
        mvaddstr(row, 2*(left + right) / 8, pCourse->getName().data());
        mvprintw(row, 3*(left + right) / 8, "%d", pCourse->getCredit());
        string teacherName;
        for (Teacher* teacher : teacherList) {
            if (pCourse->getTeacherId() == teacher->getId()) {
                teacherName = teacher->getName();
                break;
            }
        }
        mvaddstr(row, 4*(left + right) / 8, teacherName.data());
        mvaddstr(row, 5*(left + right) / 8, pCourse->getSecTime().data());
        string tmp = to_string(pCourse->getStuNum()) + "/" + to_string(pCourse->getMaxNum());
        mvaddstr(row, 6*(left + right) / 8, tmp.data());
    }
    refresh();
}

void StudentSystem::showTeacherCheckListMenu(const Teacher* p) {
    clear();
    int width = right - left;
    string str1(width, '*');
	mvaddstr(up, left, str1.data());
    string str2(width, ' ');
    str2[0] = '*'; str2[width - 1] = '*';
    for (int row = up + 1; row < down; row++) {
        mvaddstr(row, left, str2.data());
    }
	mvaddstr(down, left, str1.data());
    string titleStr = "查看选课名单";
    mvaddstr(up + 3, (left + right) / 2 - (int)titleStr.size() / 2, titleStr.data());
    mvaddstr(up + 5, (left + right) / 2 - 8, "请输入课程编号: ");
    refresh();
}

void StudentSystem::showTeacherScoreMenu(const Teacher* p) {
    clear();
    int width = right - left;
    string str1(width, '*');
	mvaddstr(up, left, str1.data());
    string str2(width, ' ');
    str2[0] = '*'; str2[width - 1] = '*';
    for (int row = up + 1; row < down; row++) {
        mvaddstr(row, left, str2.data());
    }
	mvaddstr(down, left, str1.data());
    string titleStr = "录入成绩";
    mvaddstr(up + 3, (left + right) / 2 - (int)titleStr.size() / 2, titleStr.data());
    mvaddstr(up + 5, (left + right) / 2 - 8, "请输入课程编号: ");
    refresh();
}

void StudentSystem::showTeacherModifyScoreMenu(const Teacher* p) {
    clear();
    int width = right - left;
    string str1(width, '*');
	mvaddstr(up, left, str1.data());
    string str2(width, ' ');
    str2[0] = '*'; str2[width - 1] = '*';
    for (int row = up + 1; row < down; row++) {
        mvaddstr(row, left, str2.data());
    }
	mvaddstr(down, left, str1.data());
    string titleStr = "修改成绩";
    mvaddstr(up + 3, (left + right) / 2 - (int)titleStr.size() / 2, titleStr.data());
    mvaddstr(up + 5, (left + right) / 2 - 8, "请输入课程编号: ");
    refresh();
}

void StudentSystem::showTeacherDeleteCourseMenu(const Teacher* p) {
    clear();
    int width = right - left;
    string str1(width, '*');
	mvaddstr(up, left, str1.data());
    string str2(width, ' ');
    str2[0] = '*'; str2[width - 1] = '*';
    for (int row = up + 1; row < down; row++) {
        mvaddstr(row, left, str2.data());
    }
	mvaddstr(down, left, str1.data());
    string stuStr = "教师: " + p->getName();
    mvaddstr(up + 3, (left + right) / 2 - (int)stuStr.size() / 2, stuStr.data());
    mvaddstr(up + 5, (left + right) / 2 - 8, "请输入删除课程编号: ");
    refresh();
}

void StudentSystem::showAdminMenu(const Admin* p) {
    clear();
    int width = right - left;
    string str1(width, '*');
	mvaddstr(up, left, str1.data());
    string str2(width, ' ');
    str2[0] = '*'; str2[width - 1] = '*';
    for (int row = up + 1; row < down; row++) {
        mvaddstr(row, left, str2.data());
    }
	mvaddstr(down, left, str1.data());
    string titleStr = "管理员: " + p->getName();
    mvaddstr(up + 3, (left + right) / 2 - (int)titleStr.size() / 2, titleStr.data());
    mvaddstr(up + 10, (left + right) / 2 - 8, "选择您的操作");
    mvaddstr(up + 19, opX1, "学生录入");
    mvaddstr(up + 19, opX1 + intervalXOp1, "教师录入");
    mvaddstr(up + 19, opX1 + 2 * intervalXOp1, "人员查看");
    mvaddstr(up + 19, opX1 + 3 * intervalXOp1, "人员删除");
    move(curY, curX);
    refresh();
}

void StudentSystem::showAdminInsertStudentMenu(const Admin* pa) {
    clear();
    int width = right - left;
    string str1(width, '*');
	mvaddstr(up, left, str1.data());
    string str2(width, ' ');
    str2[0] = '*'; str2[width - 1] = '*';
    for (int row = up + 1; row < down; row++) {
        mvaddstr(row, left, str2.data());
    }
	mvaddstr(down, left, str1.data());
    string titleStr = "学生录入";
    mvaddstr(up + 3, (left + right) / 2 - (int)titleStr.size() / 2, titleStr.data());
    mvaddstr(up + 5, (left + right) / 2 - 8, "请输入学生信息");
    mvaddstr(up + 7, (left + right) / 2 - 14, "学号:");
    mvaddstr(up + 8, (left + right) / 2 - 14, "姓名:");
    refresh();
}

void StudentSystem::showAdminInsertTeacherMenu(const Admin* pa) {
    clear();
    int width = right - left;
    string str1(width, '*');
	mvaddstr(up, left, str1.data());
    string str2(width, ' ');
    str2[0] = '*'; str2[width - 1] = '*';
    for (int row = up + 1; row < down; row++) {
        mvaddstr(row, left, str2.data());
    }
	mvaddstr(down, left, str1.data());
    string titleStr = "教师录入";
    mvaddstr(up + 3, (left + right) / 2 - (int)titleStr.size() / 2, titleStr.data());
    mvaddstr(up + 5, (left + right) / 2 - 8, "请输入教师信息");
    mvaddstr(up + 7, (left + right) / 2 - 14, "工号:");
    mvaddstr(up + 8, (left + right) / 2 - 14, "姓名:");
    refresh();
}

void StudentSystem::showAdminCheckPersonMenu(const Admin* pa) {
    clear();
    int width = right - left;
    string str1(width, '*');
	mvaddstr(up, left, str1.data());
    string str2(width, ' ');
    str2[0] = '*'; str2[width - 1] = '*';
    for (int row = up + 1; row < down; row++) {
        mvaddstr(row, left, str2.data());
    }
	mvaddstr(down, left, str1.data());
    string titleStr = "人员查看";
    mvaddstr(up + 3, (left + right) / 2 - (int)titleStr.size() / 2, titleStr.data());
    mvaddstr(up + 5, (left + right) / 3 - 2, "学生列表");
    mvaddstr(up + 5, 2 * (left + right) / 3 - 2, "教师列表");
    for (int i = 0; i < pa->getStudentListSize(); i++) {
        const Student* pst = pa->getStudentByIdx(i);
        assert(pst != nullptr);
        mvprintw(up + 6 + i, (left + right) / 3 - 8, "%d", pst->getId());
        mvaddstr(up + 6 + i, (left + right) / 3 + 2, pst->getName().c_str());
    }
    for (int i = 0; i < pa->getTeacherListSize(); i++) {
        const Teacher* pst = pa->getTeacherByIdx(i);
        assert(pst != nullptr);
        mvprintw(up + 6 + i, 2 * (left + right) / 3 - 5, "%d", pst->getId());
        mvaddstr(up + 6 + i, 2 * (left + right) / 3 + 2, pst->getName().c_str());
    }
    refresh();
}

void StudentSystem::showAdminDeletePersonMenu(const  Admin* pa) {
    clear();
    int width = right - left;
    string str1(width, '*');
	mvaddstr(up, left, str1.data());
    string str2(width, ' ');
    str2[0] = '*'; str2[width - 1] = '*';
    for (int row = up + 1; row < down; row++) {
        mvaddstr(row, left, str2.data());
    }
	mvaddstr(down, left, str1.data());
    string titleStr = "人员删除";
    mvaddstr(up + 3, (left + right) / 2 - (int)titleStr.size() / 2, titleStr.data());
    mvaddstr(up + 5, (left + right) / 2 - 8, "请输入删除人员学号/工号:");
    refresh();
}

void StudentSystem::run() {
    cout << "Welcome to Student System!" << endl;
    setlocale(LC_ALL,"");
	initscr();  
	crmode();  
	noecho(); 
	clear();
	keypad(stdscr, true);
	refresh();
    showStartMenu();
    state = START;
    int input = 0;
    while ((input = getch()) && (input != ERR) && (input != 'q')) {
        if (state == START) {
            string passwd;
            char str1[20], str2[20];
            int id = 0;
            bool login = false;
            switch (input) {
            case KEY_LEFT:  curX = max(studentX, curX - intervalX); move(curY, curX); break;
            case KEY_RIGHT: curX = min(studentX + 2 * intervalX, curX + intervalX); move(curY, curX); break;
            case '\n':
                /* log in */
                echo();
                nocbreak();
                mvscanw(up + 17, (left + right) / 2 - 5, "%d", &id);
                noecho();
                mvscanw(up + 19, (left + right) / 2 - 5, "%s", str2);
                cbreak();
                passwd = string(str2);
                if (curX == studentX) {
                    for (Student* pst : studentList) {
                        if (pst->getId() == id) {
                            if (pst->checkPassword(passwd)) {
                                pStudent = pst;
                                role = ROLE_STUDENT;
                                login = true;
                            }
                            break;
                        }
                    }
                    if (not login) {
                        showStartMenu();
                        mvaddstr(up + 14, (left + right) / 2 - 16, "账号密码输入错误，请重新输入!");
                        curY = up + 10, curX = studentX;
                        move(curY, curX);
                        refresh();
                        break;
                    }
                    assert(pStudent != nullptr);
                    state = STUDENT;
                    curY = up + 19, curX = opX0;
                    showStudentMenu(pStudent);
                }
                else if (curX == studentX + intervalX) { 
                    for (Teacher* pt : teacherList) {
                        if (pt->getId() == id) {
                            if (pt->checkPassword(passwd)) {
                                pTeacher = pt;
                                role = ROLE_TEACHER;
                                login = true;
                            }
                            break;
                        }
                    }
                    if (not login) {
                        showStartMenu();
                        mvaddstr(up + 14, (left + right) / 2 - 16, "账号密码输入错误，请重新输入!");
                        curY = up + 10, curX = studentX;
                        move(curY, curX);
                        refresh();
                        break;
                    }
                    assert(pTeacher != nullptr);
                    state = TEACHER;
                    curY = up + 16, curX = opX1;
                    showTeacherMenu(pTeacher);
                }
                else if (curX == studentX + 2 * intervalX) {
                    for (Admin* pa : adminList) {
                        if (pa->getId() == id) {
                            if (pa->checkPassword(passwd)) {
                                pAdmin = pa;
                                role = ROLE_ADMIN;
                                login = true;
                            }
                            break;
                        }
                    }
                    if (not login) {
                        showStartMenu();
                        mvaddstr(up + 14, (left + right) / 2 - 16, "账号密码输入错误，请重新输入!");
                        curY = up + 10, curX = studentX;
                        move(curY, curX);
                        refresh();
                        break;
                    }
                    assert(pAdmin != nullptr);
                    state = ADMIN;
                    curY = up + 19, curX = opX1;
                    showAdminMenu(pAdmin);
                }
                else assert(false);
                break;
            default: break;
            }
        } else if (state == STUDENT) {
            switch (input) {
            case KEY_LEFT:  curX = max(opX0, curX - intervalXOp0); move(curY, curX); break;
            case KEY_RIGHT: curX = min(opX0 + 4 * intervalXOp0, curX + intervalXOp0); move(curY, curX); break;
            case '\n': 
                if (curX == opX0) {
                    /* 查看选课表 */
                    showStudentcheckCourseMenu(pStudent);
                    while ((input = getch()) and (input != ERR) and (input != 'q') and (input != '\n')) {
                    }
                    curY = up + 19, curX = opX0;
                    showStudentMenu(pStudent);
                } else if (curX == opX0 + intervalXOp0) {
                    /* 查看已选课程 */
                    showStudentSelectedCourseMenu(pStudent);
                    while ((input = getch()) and (input != ERR) and (input != 'q') and (input != '\n')) {
                    }
                    curY = up + 19, curX = opX0 + intervalXOp0;
                    showStudentMenu(pStudent);
                } else if (curX == opX0 + 2 * intervalXOp0) {
                    /* 选课 */
                    showStudentAddCourseMenu(pStudent);
                    Course* pc = nullptr;
                    while ((input = getch()) and (input != ERR) and (input != 'q')) {
                        switch (input) {
                        case KEY_UP: curY = max(up + 6, curY - 1); move(curY, curX); break;
                        case KEY_DOWN: curY = min(up + 5 + (int)courseList.size(), curY + 1); move(curY, curX); break;
                        case '\n':
                            assert(curY-up-6 < (int)courseList.size());
                            pc = courseList[curY-up-6];
                            if (not pStudent->addCourse(pc)) {
                            //    assert(false);
                            }
                            break;
                        default: break;
                        }
                        if (pc != nullptr) {
                            break;
                        }
                    }
                    curY = up + 19, curX = opX0 + 2 * intervalXOp0;
                    showStudentMenu(pStudent);
                } else if (curX == opX0 + 3 * intervalXOp0) {
                    /* 退课 */
                    showStudentDropCourseMenu(pStudent);
                    Course* pc = nullptr;
                    bool select = false;
                    while ((input = getch()) and (input != ERR) and (input != 'q')) {
                        switch (input) {
                        case KEY_UP: curY = max(up + 6, curY - 1); move(curY, curX); break;
                        case KEY_DOWN: curY = min(up + 5 + (int)pStudent->getCourseList().size(), curY + 1); move(curY, curX); break;
                        case '\n':
                            select = true;
                            if (curY-up-6 >= (int)pStudent->getCourseList().size()) {
                                break;
                            }
                            pc = pStudent->getCourseList()[curY-up-6];
                            if (not pStudent->dropCourse(pc->getId())) {
                                assert(false);
                            }
                            break;
                        default: break;
                        }
                        if (select) {
                            break;
                        }
                    }
                    curY = up + 19, curX = opX0 + 3 * intervalXOp0;
                    showStudentMenu(pStudent);
                } else if (curX == opX0 + 4 * intervalXOp0) {
                    /* 查询成绩 */
                    showStudentcheckScoreMenu(pStudent);
                    while ((input = getch()) and (input != ERR) and (input != 'q') and (input != '\n')) {
                    }
                    curY = up + 19, curX = opX0 + 4 * intervalXOp0;
                    showStudentMenu(pStudent);
                } else {
                    assert(false);
                }
                break;
            default: break;
            }
        } else if (state == TEACHER) {
            switch (input) {
            case KEY_LEFT:  curX = max(opX1, curX - intervalXOp1); move(curY, curX); break;
            case KEY_RIGHT: curX = min(opX1 + 3 * intervalXOp1, curX + intervalXOp1); move(curY, curX); break;
            case KEY_UP:    curY = max(up + 16, curY - 3);  move(curY, curX); break;
            case KEY_DOWN:  curY = min(up + 19, curY + 3);  move(curY, curX); break;
            case '\n':
                if (curY == up + 16 and curX == opX1) {
                    /* 发布课程 */
                    showTeacherAddCourseMenu(pTeacher);
                    echo();
                    nocbreak();
                    char courseNameStr[20], courseTimeStr[20];
                    int courseCredit = 0, courseCapacity = 0;
                    mvscanw(up + 7, (left + right) / 2 - 5, "%s", courseNameStr);
                    mvscanw(up + 8, (left + right) / 2 - 5, "%s", courseTimeStr);
                    mvscanw(up + 9, (left + right) / 2 - 5, "%d", &courseCredit);
                    mvscanw(up + 10, (left + right) / 2 - 5, "%d", &courseCapacity);
                    noecho();
                    cbreak();
                    pTeacher->addCourse(string(courseNameStr), string(courseTimeStr), courseCredit, courseCapacity);
                    mvaddstr(up + 17, (left + right) / 2 - 16, "添加课程成功！");
                    while ((input = getch()) and (input != ERR) and (input != 'q') and (input != '\n')) {
                    }
                    showTeacherMenu(pTeacher);
                } else if (curY == up + 16 and curX == opX1 + intervalXOp1) {
                    // 查看课程
                    showTeacherCheckCourseMenu(pTeacher);
                    while ((input = getch()) and (input != ERR) and (input != 'q') and (input != '\n')) {
                    }
                    showTeacherMenu(pTeacher);
                } else if (curY == up + 16 and curX == opX1 + 2*intervalXOp1) {
                    /* 修改课程 */
                    showTeacherModifyCourseMenu(pTeacher);
                    echo();
                    nocbreak();
                    int courseId = 0;
                    mvscanw(up + 5, (left + right) / 2 + 1, "%d", &courseId);
                    Course* pc = pTeacher->getCourseById(courseId);
                    if (pc == nullptr) {
                        mvaddstr(up + 8, (left + right) / 2 - 8, "该课程ID不存在!");
                        noecho();
                        cbreak();
                        while ((input = getch()) and (input != ERR) and (input != 'q') and (input != '\n')) {
                        }
                        curY = up + 16, curX = opX1 + 2*intervalXOp1;
                        move(curY, curX);
                        showTeacherMenu(pTeacher);
                    } else {
                        mvaddstr(up + 7, (left + right) / 2 - 16, "课程名:");
                        mvaddstr(up + 7, (left + right) / 2 - 1, pc->getName().c_str());
                        mvaddstr(up + 8, (left + right) / 2 - 16, "上课时间:");
                        mvaddstr(up + 8, (left + right) / 2 - 1, pc->getSecTime().c_str());
                        mvaddstr(up + 9, (left + right) / 2 - 16, "学分:");
                        mvprintw(up + 9, (left + right) / 2 - 1, "%d", pc->getCredit());
                        mvaddstr(up + 10, (left + right) / 2 - 16, "课程容量:");
                        mvprintw(up + 10, (left + right) / 2 - 1, "%d", pc->getMaxNum());
                        mvaddstr(up + 11, (left + right) / 2 - 1, "OK");
                        noecho();
                        cbreak();
                        curY = up + 7, curX = (left + right) / 2 - 1;
                        move(curY, curX);
                        bool ok = false;
                        while ((input = getch()) and (input != ERR) and (input != 'q')) {
                            switch(input) {
                            case KEY_UP:    curY = max(up + 7, curY - 1); move(curY, curX); break;
                            case KEY_DOWN:  curY = min(up + 11, curY + 1); move(curY, curX); break;
                            case '\n': 
                                if (curY == up + 11) {
                                    ok = true;
                                } else if (curY == up + 10){
                                    int capacity = 0;
                                    echo();
                                    nocbreak();
                                    mvscanw(curY, curX + 12,"%d", &capacity);
                                    while (not pc->resetCapacity(capacity)) {
                                        /* 容量小于学生数，重新输入 */
                                        mvaddstr(curY, curX + 12, "       ");
                                        refresh();
                                        mvscanw(curY, curX + 12, "%d", &capacity);
                                    }
                                    noecho();
                                    cbreak();
                                    ++curY;
                                    move(curY, curX);
                                } else if (curY == up + 9) {
                                    int credit = 0;
                                    echo();
                                    nocbreak();
                                    mvscanw(curY, curX + 12,"%d", &credit);
                                    pc->resetCredit(credit);
                                    noecho();
                                    cbreak();
                                    ++curY;
                                    move(curY, curX);
                                } else if (curY == up + 8) {
                                    char secTime[20];
                                    echo();
                                    nocbreak();
                                    mvscanw(curY, curX + 12,"%s", secTime);
                                    pc->resetSecTime(string(secTime));
                                    noecho();
                                    cbreak();
                                    ++curY;
                                    move(curY, curX);
                                } else if (curY == up + 7) {
                                    char courseName[20];
                                    echo();
                                    nocbreak();
                                    mvscanw(curY, curX + 12,"%s", courseName);
                                    pc->resetName(string(courseName));
                                    noecho();
                                    cbreak();
                                    ++curY;
                                    move(curY, curX);
                                } else {
                                    assert(0);
                                }
                                break;
                            }
                            if (ok) {
                                break;
                            }
                        }
                        curY = up + 16, curX = opX1 + 2*intervalXOp1;
                        move(curY, curX);
                        showTeacherMenu(pTeacher);
                    }
                } else if (curY == up + 16 and curX == opX1 + 3*intervalXOp1) {
                    /* 删除课程 */
                    showTeacherDeleteCourseMenu(pTeacher);
                    echo();
                    nocbreak();
                    int courseId = 0;
                    mvscanw(up + 6, (left + right) / 2 - 3,"%d", &courseId);
                    noecho();
                    cbreak();
                    if (pTeacher->deleteCourse(courseId)) {
                        mvaddstr(up + 15, (left + right) / 2 - 10, "删除课程成功！");
                    } else {
                        mvaddstr(up + 15, (left + right) / 2 - 15, "该课程编号不存在！");
                    }
                    while ((input = getch()) and (input != ERR) and (input != 'q') and (input != '\n')) {
                    }
                    showTeacherMenu(pTeacher);
                } else if (curY == up + 19 and curX == opX1) {
                    /* 查询名单 */
                    showTeacherCheckListMenu(pTeacher);
                    echo();
                    nocbreak();
                    int courseId = 0;
                    mvscanw(up + 6, (left + right) / 2 - 3,"%d", &courseId);
                    noecho();
                    cbreak();
                    const Course* pCourse = pTeacher->getCourseById(courseId);
                    move(up + 8, (left + right) / 2 - 8);
                    if (pCourse != nullptr) {
                        for (int i = 0; i < (int)pCourse->studentList.size(); i++) {
                            mvprintw(up + 8 + i, (left + right) / 2 - 12, "%d", pCourse->studentList[i].id);
                            if (pCourse->studentList[i].score == -1) {
                                mvaddstr(up + 8 + i, (left + right) / 2 - 2, "未录入");
                            } else {
                                assert(pCourse->studentList[i].score >= 0 and pCourse->studentList[i].score <= 100);
                                mvprintw(up + 8 + i, (left + right) / 2 - 2,"%d", pCourse->studentList[i].score);
                            }
                        }
                    } else {
                        mvaddstr(up + 15, (left + right) / 2 - 15, "该课程编号不存在！");
                    }
                    while ((input = getch()) and (input != ERR) and (input != 'q') and (input != '\n')) {
                    }
                    showTeacherMenu(pTeacher);
                } else if (curY == up + 19 and curX == opX1 + intervalXOp1) {
                    /* 录入成绩 */
                    showTeacherCheckListMenu(pTeacher);
                    echo();
                    nocbreak();
                    int courseId = 0;
                    mvscanw(up + 6, (left + right) / 2 - 3,"%d", &courseId);
                    noecho();
                    cbreak();
                    Course* pCourse = pTeacher->getCourseById(courseId);
                    move(up + 8, (left + right) / 2 - 8);
                    if (pCourse != nullptr) {
                        for (int i = 0; i < (int)pCourse->studentList.size(); i++) {
                            mvprintw(up + 8 + i, (left + right) / 2 - 12, "%d", pCourse->studentList[i].id);
                        }
                        echo();
                        nocbreak();
                        int score = 0;
                        int i = 0;
                        for (; i < (int)pCourse->studentList.size(); i++) {
                            mvscanw(up + 8 + i, (left + right) / 2 - 2,"%d", &score);
                            while (score > 100 or score < 0) {
                                /* 输入不合法，重新输入 */
                                mvaddstr(up + 8 + i, (left + right) / 2 - 2, "       ");
                                refresh();
                                mvscanw(up + 8 + i, (left + right) / 2 - 2,"%d", &score);
                            }
                            pCourse->setStudentScore(i, pCourse->studentList[i].id, score);
                        }
                        move(up + 8 + i, (left + right) / 2 - 12);
                        noecho();
                        cbreak();
                    } else {
                        mvaddstr(up + 15, (left + right) / 2 - 15, "该课程编号不存在！");
                    }
                    while ((input = getch()) and (input != ERR) and (input != 'q') and (input != '\n')) {
                    }
                    showTeacherMenu(pTeacher);
                } else if (curY == up + 19 and curX == opX1 + 2*intervalXOp1) {
                    /* 修改成绩 */
                    showTeacherCheckListMenu(pTeacher);
                    echo();
                    nocbreak();
                    int courseId = 0;
                    mvscanw(up + 6, (left + right) / 2 - 3,"%d", &courseId);
                    noecho();
                    cbreak();
                    Course* pCourse = pTeacher->getCourseById(courseId);
                    move(up + 8, (left + right) / 2 - 8);
                    if (pCourse != nullptr) {
                        int i = 0;
                        for (; i < (int)pCourse->studentList.size(); i++) {
                            mvprintw(up + 8 + i, (left + right) / 2 - 12, "%d", pCourse->studentList[i].id);
                            if (pCourse->studentList[i].score == -1) {
                                mvaddstr(up + 8 + i, (left + right) / 2 - 2, "未录入");
                            } else {
                                assert(pCourse->studentList[i].score >= 0 and pCourse->studentList[i].score <= 100);
                                mvprintw(up + 8 + i, (left + right) / 2 - 2,"%d", pCourse->studentList[i].score);
                            }
                        }
                        mvaddstr(up + 8 + i, (left + right) / 2 - 2, "OK");
                        curY = up + 8, curX = (left + right) / 2 - 2;
                        move(curY, curX);
                        bool ok = false;
                        int score = 0;
                        while ((input = getch()) and (input != ERR) and (input != 'q')) {
                            switch(input) {
                            case KEY_UP:    curY = max(up + 8, curY - 1); move(curY, curX); break;
                            case KEY_DOWN:  curY = min(up + 8 + i, curY + 1); move(curY, curX); break;
                            case '\n': 
                                if (curY == up + 8 + i) {
                                    ok = true;
                                    break;
                                } else {
                                    echo();
                                    nocbreak();
                                    mvscanw(curY, curX + 6,"%d", &score);
                                    while (score > 100 or score < 0) {
                                        /* 输入不合法，重新输入 */
                                        mvaddstr(curY, curX + 6, "       ");
                                        refresh();
                                        mvscanw(curY, curX + 6, "%d", &score);
                                    }
                                    pCourse->setStudentScore(curY - up - 8, pCourse->studentList[curY - up - 8].id, score);
                                    noecho();
                                    cbreak();
                                    ++curY;
                                    move(curY, curX);
                                }
                            }
                            if (ok) {
                                break;
                            }
                        }
                    } else {
                        mvaddstr(up + 15, (left + right) / 2 - 15, "该课程编号不存在！");
                        while ((input = getch()) and (input != ERR) and (input != 'q') and (input != '\n')) {
                        }
                    }
                    curY = up + 19, curX = opX1 + 2 * intervalXOp1;
                    showTeacherMenu(pTeacher);
                } else {
                    assert(false);
                }
                break;
            default: break;
            }
        } else if (state == ADMIN) {
            switch (input) {
            case KEY_LEFT:  curX = max(opX1, curX - intervalXOp1); move(curY, curX); break;
            case KEY_RIGHT: curX = min(opX1 + 3 * intervalXOp1, curX + intervalXOp1); move(curY, curX); break;
            case '\n':
                if (curX == opX1) {
                    /* 学生录入 */
                    showAdminInsertStudentMenu(pAdmin);
                    echo();
                    nocbreak();
                    int stuId = 0;
                    char stuName[20];
                    mvscanw(up + 7, (left + right) / 2 - 6,"%d", &stuId);
                    mvscanw(up + 8, (left + right) / 2 - 6,"%s", stuName);                    
                    noecho();
                    cbreak();
                    if (pAdmin->addStudent(stuId, string(stuName))) {
                        mvaddstr(up + 17, (left + right) / 2 - 16, "学生录入成功！");
                    } else {
                        mvaddstr(up + 17, (left + right) / 2 - 16, "该学号已存在！");
                    }
                    while ((input = getch()) and (input != ERR) and (input != 'q') and (input != '\n')) {
                    }
                    showAdminMenu(pAdmin);
                } else if (curX == opX1 + intervalXOp1) {
                    /* 教师录入 */
                    showAdminInsertTeacherMenu(pAdmin);
                    echo();
                    nocbreak();
                    int teaId = 0;
                    char teaName[20];
                    mvscanw(up + 7, (left + right) / 2 - 6,"%d", &teaId);
                    mvscanw(up + 8, (left + right) / 2 - 6,"%s", teaName);                    
                    noecho();
                    cbreak();
                    if (pAdmin->addTeacher(teaId, string(teaName))) {
                        mvaddstr(up + 17, (left + right) / 2 - 16, "教师录入成功！");
                    } else {
                        mvaddstr(up + 17, (left + right) / 2 - 16, "该工号已存在！");
                    }
                    while ((input = getch()) and (input != ERR) and (input != 'q') and (input != '\n')) {
                    }
                    showAdminMenu(pAdmin);
                } else if (curX == opX1 + 2 * intervalXOp1) {
                    /* 人员查看 */
                    showAdminCheckPersonMenu(pAdmin);
                    while ((input = getch()) and (input != ERR) and (input != 'q') and (input != '\n')) {
                    }
                    showAdminMenu(pAdmin);
                } else if (curX == opX1 + 3 * intervalXOp1) {
                    /* 人员删除 */
                    showAdminDeletePersonMenu(pAdmin);
                    echo();
                    nocbreak();
                    int id = 0;
                    mvscanw(up + 6, (left + right) / 2 - 3,"%d", &id);
                    noecho();
                    cbreak();
                    if (pAdmin->deletePerson(id)) {
                        mvaddstr(up + 15, (left + right) / 2 - 10, "删除人员成功！");
                    } else {
                        mvaddstr(up + 15, (left + right) / 2 - 10, "该人员不存在！");
                    }
                    while ((input = getch()) and (input != ERR) and (input != 'q') and (input != '\n')) {
                    }
                    showAdminMenu(pAdmin);
                } else {
                    assert(false);
                }
                break;
            default: break;
            }
        } else {
            assert(false);
        }
    }
    endwin();
    if (role == ROLE_STUDENT) {
        ofstream studentStream(stuFile, ios::out);
        if (studentStream.fail()) {
            cout << "Fail to open student file" << endl;
            assert(false);
        }
        for (Student* pst : studentList) {
            studentStream << pst->getId() << "," << pst->getName() << "," << pst->getPassword() << ",";
            if (not pst->courseList.empty()) {
                for (int i = 0; i < (int)pst->courseList.size() - 1; i++) {
                    Course* pc = pst->courseList[i];
                    studentStream << pc->getId() << ":" << pc->getScoreByStudentId(pst->getId()) << "|";
                }
                studentStream << pst->courseList.back()->getId() << ":" << pst->courseList.back()->getScoreByStudentId(pst->getId());
            } else {
                studentStream << "-";
            }
            studentStream << endl;
        }
        studentStream.close();
    } else if (role == ROLE_TEACHER) {
        ofstream studentStream(stuFile, ios::out), teacherStream(teaFile, ios::out), courseStream(crsFile, ios::out);
        if (studentStream.fail()) {
            cout << "Fail to open student file" << endl;
            assert(false);
        }
        if (teacherStream.fail()) {
            cout << "Fail to open teacher file" << endl;
            assert(false);
        }
        if (courseStream.fail()) {
            cout << "Fail to open course file" << endl;
            assert(false);
        }
        for (Student* pst : studentList) {
            studentStream << pst->getId() << "," << pst->getName() << "," << pst->getPassword() << ",";
            if (not pst->courseList.empty()) {
                for (int i = 0; i < (int)pst->courseList.size() - 1; i++) {
                    Course* pc = pst->courseList[i];
                    studentStream << pc->getId() << ":" << pc->getScoreByStudentId(pst->getId()) << "|";
                }
                studentStream << pst->courseList.back()->getId() << ":" << pst->courseList.back()->getScoreByStudentId(pst->getId());
            } else {
                studentStream << "-";
            }
            studentStream << endl;
        }
        studentStream.close();
        for (Teacher* pt : teacherList) {
            teacherStream << pt->getId() << "," << pt->getName() << "," << pt->getPassword() << ",";
            if (not pt->courseList.empty()) {
                for (int i = 0; i < (int)pt->courseList.size() - 1; i++) {
                    Course* pc = pt->courseList[i];
                    teacherStream << pc->getId() << "|";
                    courseStream << pc->getId() << "," << pc->getName() << "," << pc->getCredit() << "," 
                    << pc->getTeacherId() << "," << pc->getSecTime() << "," << pc->getMaxNum() << endl;
                }
                Course* pc = pt->courseList.back();
                teacherStream << pc->getId();
                courseStream << pc->getId() << "," << pc->getName() << "," << pc->getCredit() << "," 
                    << pc->getTeacherId() << "," << pc->getSecTime() << "," << pc->getMaxNum() << endl;
            } else {
                teacherStream << "-";
            }
            teacherStream << endl;
        }
        courseStream.close();
        teacherStream.close();
    } else if (role == ROLE_ADMIN) {
        ofstream studentStream(stuFile, ios::out), teacherStream(teaFile, ios::out), courseStream(crsFile, ios::out);
        if (studentStream.fail()) {
            cout << "Fail to open student file" << endl;
            assert(false);
        }
        if (teacherStream.fail()) {
            cout << "Fail to open teacher file" << endl;
            assert(false);
        }
        if (courseStream.fail()) {
            cout << "Fail to open course file" << endl;
            assert(false);
        }
        for (Student* pst : pAdmin->studentList) {
            studentStream << pst->getId() << "," << pst->getName() << "," << pst->getPassword() << ",";
            if (not pst->courseList.empty()) {
                for (int i = 0; i < (int)pst->courseList.size() - 1; i++) {
                    Course* pc = pst->courseList[i];
                    studentStream << pc->getId() << ":" << pc->getScoreByStudentId(pst->getId()) << "|";
                }
                studentStream << pst->courseList.back()->getId() << ":" << pst->courseList.back()->getScoreByStudentId(pst->getId());
            } else {
                studentStream << "-";
            }
            studentStream << endl;
        }
        studentStream.close();
        for (Teacher* pt : pAdmin->teacherList) {
            teacherStream << pt->getId() << "," << pt->getName() << "," << pt->getPassword() << ",";
            if (not pt->courseList.empty()) {
                for (int i = 0; i < (int)pt->courseList.size() - 1; i++) {
                    Course* pc = pt->courseList[i];
                    teacherStream << pc->getId() << "|";
                    courseStream << pc->getId() << "," << pc->getName() << "," << pc->getCredit() << "," 
                    << pc->getTeacherId() << "," << pc->getSecTime() << "," << pc->getMaxNum() << endl;
                }
                Course* pc = pt->courseList.back();
                teacherStream << pc->getId();
                courseStream << pc->getId() << "," << pc->getName() << "," << pc->getCredit() << "," 
                    << pc->getTeacherId() << "," << pc->getSecTime() << "," << pc->getMaxNum() << endl;
            } else {
                teacherStream << "-";
            }
            teacherStream << endl;
        }
        courseStream.close();
        teacherStream.close();
    } else {
        assert(role == ROLE_DUMMY);
    }
}