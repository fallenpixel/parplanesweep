 
OPTFLAGS = -O3

CCC=g++ -std=c++11 

SRCMAPALGEBRA = ../map/hseg2DFixedSize.cpp ../map/poi2DFixedSize.cpp ../map/seg2DFixedSize.cpp ../map/mbb2DFixedSize.cpp
INCLUDEMAPALGEBRA = -I ../map

all: pps
	 $(info ***** be sure to set lib path to current dir with: export LD_LIBRARY_PATH=.)
	 $(info ***** thrtead affinity env variable: export GOMP_CPU_AFFINITY=0-x, x = num processors)


pps: main.o  libparOverlay.so
	${CCC} ${OPTFLAGS} -o pps -fopenmp -L ./ main.o -l parOverlay -l tbb



libparOverlay.so:   parPlaneSweep.o
	${CCC} -fopenmp -shared -Wl,-soname,libparOverlay.so.1   -o libparOverlay.so.1.0.1 parPlaneSweep.o 
	ln -f -s libparOverlay.so.1.0.1 libparOverlay.so
	ldconfig  -n .

main.o: main.cpp
	${CCC} ${OPTFLAGS} -c main.cpp 


parPlaneSweep.o: parPlaneSweep.h parPlaneSweep.cpp
	${CCC} ${OPTFLAGS} -fPIC -fopenmp -c parPlaneSweep.cpp


check-syntax:
	${CCC} -o /dev/null -S ${CHK_SOURCES}

clean:
	rm *.o
	rm *.so*
