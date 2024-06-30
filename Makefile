OPTIONS = -std=c++17 -O0 -g -Wall -Wextra -I include/
all: build/test 

build/test: tools/test.cpp
	g++ ${OPTIONS} tools/test.cpp -o build/test

clean: 
	rm -rf build/*.o build/*