all:
	g++ -Wall -Isrc/portal -Isrc/states -Isrc/engine -Isrc/io src/portal/*.cpp src/engine/*.cpp src/states/*.cpp src/io/*.cpp src/main.cpp -o release/student_portal.out -lncurses
