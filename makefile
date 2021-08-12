all: noir-sunrise clean

noir-sunrise: main.o generator.o behavior.o entity.o mapfunctions.o
	g++ main.o generator.o behavior.o entity.o mapfunctions.o -o noir-sunrise -lpthread

main.o: main.cpp
	g++ -c main.cpp

generator.o: generator.cpp
	g++ -c generator.cpp

behavior.o: behavior.cpp
	g++ -c behavior.cpp

entity.o: entity.cpp
	g++ -c entity.cpp

mapfnunctions.o: mapfunctions.o
	g++ -c mapfunctions.cpp

clean:
	rm -rf *.o
