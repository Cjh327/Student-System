# Student System
### 一、课设题目

信息管理系统: 即实现⼀个基于控制台的信息管理系统，例如选课系统、图书馆系统、公司财务系统等等 ，
除满⾜增删查改等基本功能，可以根据实际使用场景自由设计功能。

实现⼀个简单的选课系统。系统主要有三个角色，管理员、教师和学生，每个角色除了基本的登录功能(载入数据)和退出功能（退出并保存数据）外，还有各自的操作。 

### 二、基本框架

学生选课系统主要由三个角色构成：管理员、教师和学生，这三种角色的用户都有一些共同的属性，比如账号名，密码等，因此可定义一个基类Person，其成员变量为这些共同属性，管理员类Admin、教师类Teacher、学生类Student都是基类Person的派生类，继承这些共有属性。此外，学生和教师都需要和课程进行交互，因此定义一个课程类Course，用来表示单个课程。

学生选课系统的架构如下所示：

![](C:\Users\tf\Desktop\大三下\高级程序设计\课程设计一\图片2.png)

可以看到，Student类，Teacher类，Admin类继承了Person类的成员变量，并且它们都有各自的新的成员变量，用来实现它们各自的功能。

### 三、具体实现

#### 数据结构

Course类的定义如下：

```C++
class Course {
private:
	int id; 			// 课程编号
	string name; 		// 课程名
	string secTime; 	// 上课时间
	int teacherId; 		// 教师工号
	int credit; 		// 学分
	int capacity; 		// 课程容量
	vector<StudentInfo> studentList; // 学生列表
public:
	Course(int, string, int, int, string, int); 	// 构造函数
	bool addStudent(int stuId, int score); 		// 添加学生
	bool eraseStudent(int stuId); 				// 删除学生
	int getId() const;  		// 获取课程编号
	string getName() const; 	// 获取课程名
	int getCredit() const; 		// 获取学分
	int getCapacity() const; 	// 获取课程容量
	int getStuNum() const;		// 获取学生数量
	int getTeacherId() const; 	// 获取教师工号
	string getSecTime() const; 	// 获取上课时间
	bool setStudentScore(int stuId, int score); 	// 录入学生成绩
	int getScoreByStudentId(int stuId); 	// 获取学生成绩
	bool resetCapacity(int); 	// 重置课程容量
	bool resetCredit(int); 		// 重置学分
	bool resetSecTime(string); 	// 重置上课时间
	bool resetName(string); 	// 重置课程名
};
```

Course类用来表示课程，它有课程编号、课程名、学分等成员变量，其中studengList存储了选择该门课程的学生列表，StudentInfo结构体存储了学生的ID和该学生这门课的成绩。为每个成员变量分配一个对外接口，此外，提供添加学生、删除学生、录入学生成绩等接口供后面的学生类和教师类使用。

Person类的定义如下：

```C++
class Person {
private:
	int id; 			// ID
	string name; 		// 姓名
	string password; 	// 密码
public:
	Person(const int, const string, const string); 	// 构造函数
	int getId() const; 		// 获取ID
	string getName() const; // 获取姓名
	bool checkPassword(const string& str) const; 	// 核对密码
	bool modifyPassword(); 	// 修改密码
};
```

Person类中包含id, name, password三个成员变量，表示用户的id、姓名和密码，这是三类用户所共有的属性。public成员函数中，定义id, name的对外接口，由于password的特殊性，不应被外部获得，因此定义checkPassword接口，用来检查传入的参数密码是否与用户的密码一致。

Student类的定义如下：

```C++
class Student: public Person {
private:
	std::vector<Course*> courseList; 	// 课程列表
public:
	Student(const int, const string, const string, const vector<Course*>&);
	Student(const int, const string, const string);
	Student(const int, const string);
	bool addCourse(Course* pCourse); 				// 添加课程
	bool deleteCourse(int courseNo); 				// 删除课程
	void dropAllCourses(); 							// 删除所有课程
};
```

Student类继承基类Person，它有自己的成员变量courseList，存储的是该学生所选的课程列表，此外定义一些外部接口，以实现学生用户的操作。

addCourse表示选课，传入参数为指向课程对象的指针，检查该课程号是否已经在courseList中出现，若已存在，则说明已经选过该课，无法重复添加，若不存在，则将其加入课程列表中，同时，在类Course中，也需要将该学生加入其学生列表中。

dropCourse表示退课，传入参数为需要删除的课程编号，同样需要在courseList中检查该课程号是否存在，若不存在，说明该学生未选过该课，无法删除，若存在，则将其从courseList中删除，同时在类Course中，也需要将该学生从studentList中删除。

dropAllCourses接口用来在删除学生用户时，从该学生的所有课程的学生列表将该学生删除。

Teacher类的定义如下：

```C++
class Teacher: public Person {
private:
    std::vector<Course*> courseList;
public:
    Teacher(const int, string, const string, const vector<Course*>&);
    Teacher(const int, string, const string);
    Teacher(const int, string);
    bool addCourse(string name, string secTime, int credit, int capacity); 	// 发布课程
    bool deleteCourse(int courseNo); 	// 删除课程
    void deleteAllCourses(); 			// 删除所有课程
    const int getCourseNum() const; 	// 获取课程数量
    Course* getCourseById(int courseId) const; 	// 根据课程ID获取课程
};
```

Teacher类继承基类Person，它有自己的成员变量courseList，存储该教师所发布的课程列表，此外定义教师类增删改查的对外接口。

发布课程：addCourse接口用来给教师发布课程使用，传入的参数为该课程的基本信息，在函数体内，为该课程分配一个唯一的课程编号，并为其创建一个课程对象，将其地址添加到courseList内。

删除课程：deleteCourse接口传入的参数为要删除的课程编号，检查该编号是否存在于该老师的课程列表中，若存在，则将其删除，同时对于选该门课的学生，也需要将其课程列表中删除该课程。

查询课程：查询课程使用getCourseById操作，传入参数为课程编号，返回该课程对象指针。

修改课程：修改课程操作同样使用getCourseById操作获取该课程对象，并在外部对其进行相应修改。

录入/修改成绩：录入成绩时通过getCourseById接口获得该门课对象的指针，调用Course类的setStudentScore接口为学生录入和修改成绩。

Admin类的定义如下：

```C++
class Admin: public Person {
private:
    vector<Student*> studentList; 	// 所有学生列表
    vector<Teacher*> teacherList; 	// 所有教师列表
public:
    Admin(int, string, string, vector<Student*>, vector<Teacher*>);
    bool addStudent(int id, string name); 	// 添加学生
    bool addTeacher(int id, string name); 	// 添加教师
    bool deletePerson(int id); 				// 删除人员（学生/教师）
    int getStudentListSize() const; 		// 获取学生数量
    const Student* getStudentByIdx(int idx) const; 	// 获取学生
    int getTeacherListSize() const; 		// 获取教师数量
    const Teacher* getTeacherByIdx(int idx) const;  // 获取教师
};
```

管理员主要的功能是管理学生和教师信息，因此Admin类的成员变量为studentList和teascherList。分别存储所有的学生列表和所有的教师列表。

管理员可以对学生和教师进行添加和删除操作，这和前述的增删操作类似，添加学生/教师时，同样都是通过ID号判断该ID是否已存在，若不存在则加入列表中。

删除人员时，遍历学生和教师列表，若存在该ID，则将其删除，如果删除的是老师，则需要将其课程也一起删除，并且选了该门课的学生需要将该门课移除，如果删除的是学生，则需要将该学生选的课的学生列表中移除该学生。

#### 数据加载和存储

用户和课程数据存储在文件中，分别为courses.txt, students.txt, teachers.txt, admins.txt，它们的存储格式为（#开头的行为注释）：

courses.txt

```
#课程编号,课程名,学分,教师工号,上课时间,课程容量
420041,数据结构,2,21002,周五三四节,150
420042,算法设计基础,4,21002,周五三四节,100
420043,软件产业概论,2,21004,周一三四节,200
```

students.txt

```
#学号,姓名,密码,课程编号1:成绩1|课程编号2:成绩2...
161220987,张三,zs12345,420041:93|420042:94
161220988,李四,lz12345,420042:94
161220999,王五,ww12345,420043:95|420042:96
```

teachers.txt

```
#工号,姓名,密码,课程编号1|课程编号2...
21002,张老师,zls12345,420042|420041
21004,周老师,21004,420043
```

admins.txt

```
#账号,姓名,密码
10000,管理员,admin
```

系统运行开始时，读取数据文件，根据登陆的用户角色为其加载其有用的数据；系统退出时，保存当前用户的有用信息，存储到数据文件中。

#### 界面

在面向用户的界面上，我使用的是Linux下的curses库，实现荧屏显示和处理。界面共分为四类。

第一类是初始登陆界面，在该界面上进行登陆角色的选择和账号密码的输入，检查账号密码是否正确，若正确，则进入该角色的界面。

![](C:\Users\tf\Desktop\大三下\高级程序设计\课程设计一\start.png)

第二类是学生界面，该界面上包含了学生用户可以进行的操作选项，用户可以自由进入各选项进行相应操作。

![](C:\Users\tf\Desktop\大三下\高级程序设计\课程设计一\student.png)

第三类是教师界面，包含了教师的各项操作。

![](C:\Users\tf\Desktop\大三下\高级程序设计\课程设计一\teacher.png)

第四类是管理员界面，为管理员的人员管理提供支持。

![](C:\Users\tf\Desktop\大三下\高级程序设计\课程设计一\admin.png)

### 四、运行说明

该程序在Linux系统下运行，编译前需要安装外部依赖curses函数库，命令为：

```shell
sudo apt-get install ncurses-dev 
```

编译运行时的命令为：

```shell
make   		# 编译
make run 	# 编译并运行
make clean	# 删除中间文件和可执行文件
```

### 五、运行结果

以学生用户为例，运行结果如下图所示：

##### 查看选课表

![](C:\Users\tf\Desktop\大三下\高级程序设计\课程设计一\checkcourse.png)

##### 查看已选课程

![](C:\Users\tf\Desktop\大三下\高级程序设计\课程设计一\selectedcourse.png)

##### 选课

选择软件产业概论课程后查看已选课程

![](C:\Users\tf\Desktop\大三下\高级程序设计\课程设计一\select2.png)

##### 退课

退选数据结构课后查看已选课程

![](C:\Users\tf\Desktop\大三下\高级程序设计\课程设计一\sle3.png)

##### 查询成绩

![](C:\Users\tf\Desktop\大三下\高级程序设计\课程设计一\score.png)

