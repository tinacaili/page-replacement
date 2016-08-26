all: vsim vmgenU vmgenWS

vsim: cse422_lab3_part1.cpp 
	g++ -std=c++11 -o vsim cse422_lab3_part1.cpp

vmgenU: cse422_lab3_part2.cpp
	g++ -std=c++11 -o vmgenU cse422_lab3_part2.cpp

vmgenWS: cse422_lab3_part3.cpp
	g++ -std=c++11 -o vmgenWS cse422_lab3_part3.cpp
