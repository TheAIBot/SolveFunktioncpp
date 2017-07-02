OBJS = omath.o SolveFunktion.o tsrandom.o
LIBS = -lm
CFLAGS = -Wall -O3 -ffast-math -pthread -flto

all: solver

%.o: %.cpp
	g++ -std=c++14 ${CFLAGS} -c $<

solver: ${OBJS}
	g++ -std=c++14 ${CFLAGS} ${OBJS} ${LIBS} -o solver
	
clean:
	rm -f ${OBJS} solver
