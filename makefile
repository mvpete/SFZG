main:
	clang++ -Wall -std=c++0x -g -c *.cpp
	clang++ *.o -o sfzg -lsfml-graphics -lsfml-window -lsfml-system

clean_temp:
	rm *~

clean:
	rm *.o