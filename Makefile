OPTIONS = -std=c++17 -O0 -g -Wall -Wextra -I include/
all: build/basic_main

build/basic_main: 
	g++ ${OPTIONS} tools/basic_main.cpp -o build/first_main

clean: 
	rm -rf build/*.o build/*