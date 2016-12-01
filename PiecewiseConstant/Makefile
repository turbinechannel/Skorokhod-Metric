CC = g++
CFLAGS = -Wall -Wextra -std=c++11


#mytest : global_defs.h global_defs.o monitor.o free-space-boundary.o myprint.o cell.o read_input_data.o mytest.cpp 
#	${CC} ${CFLAGS} global_defs.o read_input_data.o monitor.o free-space-boundary.o myprint.o cell.o mytest.cpp  -o mytest

skorokhod_monitor : global_defs.h global_defs.o monitor.o  myprint.o read_input_data.o pdist.o skorokhod_monitor.cpp
	${CC} ${CFLAGS} global_defs.o read_input_data.o monitor.o myprint.o pdist.o skorokhod_monitor.cpp  -o skorokhod_monitor


monitor.o : global_defs.h monitor.h point.h    pdist.h global_defs.o monitor.cpp
	${CC} ${CFLAGS} -c  monitor.cpp

#mytest : global_defs.o pdist.o myprint.o read_input_data.o global_defs.o mytest.cpp
#		${CC} ${CFLAGS} myprint.o pdist.o mytest.cpp -o mytest

pdist.o : point.h pdist.h pdist.cpp
	${CC} ${CFLAGS} -c  pdist.cpp


myprint.o : myprint.cpp myprint.h  global_defs.h
	${CC} ${CFLAGS} -c  myprint.cpp


read_input_data.o : global_defs.h read_input_data.h read_input_data.cpp point.h
	${CC} ${CFLAGS} -c read_input_data.cpp

global_defs.o : global_defs.h global_defs.cpp
	${CC} ${CFLAGS} -c global_defs.cpp
