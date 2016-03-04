 
OPTFLAGS = -O3

CCC=g++

all: pps
	 $(info ***** be sure to set lib path to current dir with: export LD_LIBRARY_PATH=.)
	 $(info ***** thrtead affinity env variable: export GOMP_CPU_AFFINITY=0-x, x = num processors)

pps: main.o  libparOverlay.so
	${CCC} ${OPTFLAGS} -o pps -fopenmp -L ./ main.o -l parOverlay



libparOverlay.so:  avl.o parPlaneSweep.o
	${CCC} -fopenmp -shared -Wl,-soname,libparOverlay.so.1   -o libparOverlay.so.1.0.1 parPlaneSweep.o avl.o
	ln -f -s libparOverlay.so.1.0.1 libparOverlay.so
	ldconfig  -n .

main.o: main.cpp
	${CCC} -fopenmp  ${OPTFLAGS} -c main.cpp 



parPlaneSweep.o: parPlaneSweep.h parPlaneSweep.cpp
	${CCC} ${OPTFLAGS} -fPIC -fopenmp -c parPlaneSweep.cpp

avl.o: avl.h avl.c
	gcc ${OPTFLAGS} -c -fPIC avl.c

check-syntax:
	${CCC} -o /dev/null -S ${CHK_SOURCES}

clean:
	rm *.o
	rm *.so*
