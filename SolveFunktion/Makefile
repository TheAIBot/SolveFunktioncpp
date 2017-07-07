OBJS = omath.o SolveFunktion.o tsrandom.o
LIBS = -lm 
#-L../../../Libraries -ltsrandom
CFLAGS = -Wall -O3 -ffast-math -pthread -flto -fuse-linker-plugin -march=native -I../../../Includes

all: solver

%.o: %.cpp
	g++ -std=c++14 ${CFLAGS} -c $<

solver: ${OBJS}
	g++ -std=c++14 ${CFLAGS} ${OBJS} ${LIBS} -o solver
	
clean:
	rm -f ${OBJS} solver
