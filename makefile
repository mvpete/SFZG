main: objects
	clang++ *.o -o sfzg -lsfml-graphics -lsfml-window -lsfml-system

objects: *.cpp
	clang++ -Wall -std=c++0x -g -c *.cpp

clean_temp:
	rm *~
clean_bin: 
	rm sfzg

clean: clean_temp clean_bin 
	rm *.o