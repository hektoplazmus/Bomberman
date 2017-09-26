PROGRAM=pancijan

CC=g++
FLAG=-std=c++11 -Wall -pedantic -Wextra -g -Wno-long-long -O0 -ggdb

all: compile doc

run: compile
	./$(PROGRAM)

compile: $(PROGRAM)

doc: src/main.cpp src/CPlayer.cpp
	doxygen Doxyfile

$(PROGRAM): objs/main.o  objs/CObjects.o objs/CPlayer.o  objs/CGame.o objs/CMap.o objs/CBomberman.o| objs   
	${CC} ${FLAG} $^ -o $@ -lncurses -lpthread

objs/CPlayer.o: src/CPlayer.cpp src/CPlayer.hpp src/CObjects.hpp  objs
	${CC} ${FLAG} -c $< -o $@

objs/CObjects.o: src/CObjects.cpp src/CObjects.hpp  objs
	${CC} ${FLAG} -c $< -o $@

objs/CGame.o: src/CGame.cpp src/CGame.hpp src/CMap.hpp src/CPlayer.hpp src/CObjects.hpp  objs
	${CC} ${FLAG} -c $< -o $@

objs/CMap.o: src/CMap.cpp src/CMap.hpp src/CObjects.hpp src/CPlayer.hpp  objs
	${CC} ${FLAG} -c $< -o $@

objs/CBomberman.o: src/CBomberman.cpp src/CBomberman.hpp src/CGame.hpp objs
	${CC} ${FLAG} -c $< -o $@

objs/main.o: src/main.cpp objs
	${CC} ${FLAG} -c $< -o $@

objs:
	mkdir objs

clean:
	rm -rf $(PROGRAM) objs/ doc/ 2>/dev/null


