CC:=g++
exe:=main
obj:=main.o studentSystem.o course.o person.o student.o teacher.o admin.o
all:$(obj)
	$(CC) -o $(exe) $(obj) -lncursesw
%.o:%.cpp
	$(CC) -std=c++11 -c $^ -o $@
.PHONY:clean
clean:
	rm -rf $(obj) $(exe)
cleanobj:
	rm -rf $(obj)
run:
	make clean
	make all
	make cleanobj
	./main
