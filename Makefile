CXX=g++
ORBI=-I${ACE_ROOT} -I${TAO_ROOT}
ORBL=-L${ACE_ROOT}/lib -lACE -lTAO -lTAO_PortableServer -lTAO_AnyTypeCode

all: bin dxcC.o dxcS.o
	${CXX} -o bin/dxc dxcC.o dxcS.o dxc.cpp ${ORBI} ${ORBL}
	${CXX} -o bin/dxcd dxcC.o dxcS.o dxcd.cpp ${ORBI} ${ORBL}

bin:
	mkdir -p bin

dxcC.h:
	tao_idl -Gmc -St -Sa -Sal dxc.idl

dxcC.o: dxcC.h bin
	${CXX} -c -o dxcC.o dxcC.cpp ${ORBI}

dxcS.o: dxcC.h bin
	${CXX} -c -o dxcS.o dxcS.cpp ${ORBI}

install:
	install bin/dxc /usr/bin/dxc
	mkdir -p /usr/lib64/dxc/bin
	ln -fs /usr/bin/dxc /usr/lib64/dxc/bin/c++
	ln -fs /usr/bin/dxc /usr/lib64/dxc/bin/cc
	ln -fs /usr/bin/dxc /usr/lib64/dxc/bin/g++
	ln -fs /usr/bin/dxc /usr/lib64/dxc/bin/gcc
	ln -fs /usr/bin/dxc /usr/lib64/dxc/bin/x86_64-pc-linux-gnu-c++
	ln -fs /usr/bin/dxc /usr/lib64/dxc/bin/x86_64-pc-linux-gnu-g++
	ln -fs /usr/bin/dxc /usr/lib64/dxc/bin/x86_64-pc-linux-gnu-gcc

clean:
	rm -Rf dxc dxcd dxcS* dxcC* bin *.o
