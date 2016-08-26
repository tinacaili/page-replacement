// cse422_lab3_part2.cpp 
// Authors: Tina Li (ttli@wustl.edu) and Megan Bacani (meganbacani@wustl.edu)
#include <iostream>
#include <ostream>
#include <string>
#include <istream>
#include <sstream>
#include <iterator>
#include <algorithm>
#include <fstream>
#include <vector>
#include <stdlib.h>
#include <time.h>


using namespace std;

int refRange;
int seqLength;

//seeds and generates random numbers seqLength times, writing into outfile
int generate(string outfile){
	srand(time(NULL));
	int random;

	ofstream ofs(outfile);
	if (ofs){
		int i = 0;
		while (i < seqLength){
			random = rand() % refRange;
			ofs << random << " ";
			i++;
		}
		ofs << endl;
	}
	ofs.close();

	return 0;
}

int usage(){
	cout << "usage: vmgenU <range_of_page_refs> <length_of_sequence> <output_file> " << endl;
	return 0;
}

int main(int argc, const char* argv[])
{
	//checks for 4 inputs (range of page refs (max 100), length of sequence, input file)
	if (argc != 4) {
		//prints usage if you give the wrong number of arguments
		usage();
		//returns number thats not 1 to reflect not a success
		return 1;
	}
	else {
		//checks refRange is a number max 100
		try {
			refRange = stoi(argv[1]);
		}
		catch (...){
			cout << "Range of page references not a number" << endl;
			usage();
			return 1;
		}
		if (refRange > 100){
			cout << "Range of page references is max 100" << endl;
			usage();
			return 1;
		}
		//checks seqLength is a number
		try {
			seqLength = stoi(argv[2]);
		}
		catch (...){
			cout << "Length of sequence not a number" << endl;
			usage();
			return 1;
		}
		string outfile = argv[3];
		generate(outfile);
	}
	return 0;
}

