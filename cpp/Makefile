# CXXFLAGS += -std=c++14

cc = g++
objects = tag.o ifd.o demo.o
exe = demo


main:$(objects)
	$(cc) $(objects) -o demo

demo.o: demo.cpp
	$(cc) -c demo.cpp
tag.o: tag.cpp
	$(cc) -c tag.cpp
ifd.o: ifd.cpp
	$(cc) -c ifd.cpp

.PHONY:clean
clean:
	rm demo $(objects)