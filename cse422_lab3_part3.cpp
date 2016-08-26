//Authors: Tina Li (ttli@wustl.edu) and Megan Bacani (meganbacani@wustl.edu)

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

void usage() {
	cout << "usage: vmgenWS <ws_size> <lower_bound> <upper_bound> <range> <sequence_length> <filename>" << endl;
	cout << "\t ws_size: the size of the working set" << endl;
	cout << "\t lower_bound: the lower bound on the number of pages to generate in a given working set exhibiting locality of reference" << endl;
	cout << "\t upper_bound: the upper bound on the number of pages to generate in a given working set exhibiting locality of reference" << endl;
	cout << "\t range: the range of page references (maximum 100)" << endl;
	cout << "\t sequence_length: the length of the sequence of page references" << endl;
	cout << "\t filename: the name of the file that will be generated" << endl;
}

int main(int argc, const char * argv[]) {
	//Check for usage
	if (argc != 7) {
		usage();
		return 2;
	}

	// Parse arguments
	int ws_size = stoi(argv[1]);
	int lower_bound = stoi(argv[2]);
	int upper_bound = stoi(argv[3]);
	int range = stoi(argv[4]);
	int sequence_length = stoi(argv[5]);
	string filename = argv[6];

	// File Stream
	ofstream ofs(filename);

	vector<int> refs;

	//seed random number generator
	srand(time(NULL));

	//generate numbers and push into the vector
	while (refs.size() < sequence_length) {

		int ws_nums = lower_bound + (rand() % (upper_bound - lower_bound));
		vector<int> ws;
		int i;
		for (i = 0; i < ws_size; i++) {
			ws.push_back(rand() % range);
		}
		for (i = 0; i < ws_nums && refs.size() < sequence_length; i++) {
			refs.push_back(ws[rand() % ws.size()]);
		}
	}

	if (ofs){
		for (int i : refs){
			ofs << i << " ";
		}
		ofs << endl;
	}
	ofs.close();

	return 0;
}

