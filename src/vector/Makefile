 
OPTFLAGS = -O3

CCC=/opt/local/bin/g++-mp-5 
#CCC=g++ -std=c++11 

SRCMAPALGEBRA = ../map/hseg2DFixedSize.cpp ../map/poi2DFixedSize.cpp ../map/seg2DFixedSize.cpp ../map/mbb2DFixedSize.cpp
INCLUDEMAPALGEBRA = -I ../map
all: pps
	 $(info ***** be sure to set lib path to current dir with: export LD_LIBRARY_PATH=.)

#pps: main.o  libparOverlay.so
#	${CCC} ${OPTFLAGS} -o pps -fopenmp -L ./ main.o -l parOverlay
#	${CCC} ${OPTFLAGS} -o ppsBin -fopenmp -L ./ mainBin.o -l parOverlay $(SRCMAPALGEBRA) 
pps: main.o  libparOverlay.so
	${CCC} ${OPTFLAGS} -o pps -fopenmp -L ./ main.o -l parOverlay -lpapi	
#	${CCC} ${OPTFLAGS} -o ppsBin -fopenmp -L ./ mainBin.o -l parOverlay $(SRCMAPALGEBRA) 


#libparOverlay.so:   parPlaneSweep.o
#	${CCC} ${OPTFLAGS} -fopenmp -dynamiclib -Wl,-dylib_install_name,libparOverlay.so.1  -o libparOverlay.so.1.0.1 parPlaneSweep.o 
#	ln -f -s libparOverlay.so.1.0.1 libparOverlay.so.1
#	ln -f -s libparOverlay.so.1.0.1 libparOverlay.so

libparOverlay.so:   parPlaneSweep.o
	${CCC} -fopenmp -shared -Wl,-soname,libparOverlay.so.1   -o libparOverlay.so.1.0.1 parPlaneSweep.o 
	ln -f -s libparOverlay.so.1.0.1 libparOverlay.so
	ldconfig  -n .

main.o: main.cpp
	${CCC} ${OPTFLAGS} -c main.cpp 



#mainBin.o: mainBin.cpp
#	${CCC} ${OPTFLAGS} -c mainBin.cpp $(INCLUDEMAPALGEBRA) 


parPlaneSweep.o: parPlaneSweep.h parPlaneSweep.cpp
	${CCC} ${OPTFLAGS} -fPIC -fopenmp -c parPlaneSweep.cpp


check-syntax:
	${CCC} -o /dev/null -S ${CHK_SOURCES}

clean:
	rm *.o
	rm *.so*
