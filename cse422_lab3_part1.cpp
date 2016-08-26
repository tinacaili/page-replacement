// cse422_lab3_part1.cpp : Defines the entry point for the console application.
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

using namespace std;

vector<int> pages;
int frameNum;

//operator overloading to make printing the vector of frames easier
ostream& operator<< (ostream & os, vector<int> frames_print){
	os << "[";
	for (int i = 0; i < frameNum; i++){
		if (i < frames_print.size()){
			if (frames_print[i] < 10) os << " " << frames_print[i];
			else os << frames_print[i];
		}
		else os << "  ";
		if (i < frameNum - 1) os << "|";
	}
	os << "]";
	return os;
}

//sees if i is in vec_target--search starts at start, returns position if in vector, else returns -1;
int contains(vector<int> vec_target, int i, int start){
	for (int j = start; j < vec_target.size(); j++){
		if (i == vec_target[j])
			return j;
	}
	return -1;
}

//replaces least recently used
int lru(){
	int num_faults = 0;
	int num_refs = 0;

	vector<int> lru_stack;
	vector<int> lru_frames;

	for (int i : pages){
		bool is_fault = false;

		//if lru frames don't contain the page
		int pos = contains(lru_frames, i, 0);
		if (pos == -1){
			//page fault, needs replacement
			if (lru_frames.size() >= frameNum){
				//finds the least recently used to replace in a stack that keeps track of what elements in the frames have been used recently
				int to_replace = lru_stack[0];
				pos = contains(lru_frames, to_replace, 0);
				lru_frames[pos] = i;

				//remove replaced in stack and put in new int
				lru_stack.erase(lru_stack.begin());
				lru_stack.push_back(i);

				is_fault = true;
				num_faults++;
				num_refs++;
			}
			else{
				lru_frames.push_back(i);
				lru_stack.push_back(i);
			}
		}
		//if lru frames contains the page
		else{
			pos = contains(lru_stack, i, 0);
			lru_stack.erase(lru_stack.begin() + pos);
			lru_stack.push_back(i);
			if (lru_frames.size() >= frameNum) num_refs++;
		}
		cout << i << ": " << lru_frames;
		if (is_fault) cout << "  F";
		cout << endl;
	}

	cout << "Miss rate = ";
	if (num_refs == 0) cout << "N/A" << endl;
	else cout << num_faults << " / " << num_refs << " = " << (num_faults / (double)num_refs) * 100 << "%" << endl;

	return 0;
}

//first page in is the first page replaced
int fifo()
{
	int num_faults = 0;
	int num_refs = 0;

	vector<int> fifo_frames;
	int nextOut = 0;
	int pf = 0;

	for (vector<int>::iterator it = pages.begin(); it != pages.end(); ++it)
	{
		bool is_fault = false;
		//did not find page
		if (contains(fifo_frames, *it, 0) == -1)
		{
			//page fault
			if (fifo_frames.size() >= frameNum)
			{
				//replaces the first page in
				fifo_frames[nextOut] = *it;
				if (nextOut + 1 < frameNum)
					nextOut++;
				else
					nextOut = 0;
				num_refs++;
				num_faults++;
				is_fault = true;
			}
			else
			{
				//continue filling frames
				fifo_frames.push_back(*it);
			}
		}
		//found page
		else {
			if (fifo_frames.size() >= frameNum) num_refs++;
		}
		cout << *it << ": " << fifo_frames;
		if (is_fault) cout << "  F";
		cout << endl;

	}

	cout << "Miss rate = ";
	if (num_refs == 0) cout << "N/A" << endl;
	else cout << num_faults << " / " << num_refs << " = " << (num_faults / (double)num_refs) * 100 << "%" << endl;


	return 0;
}

//looks to find the one not used in the furthest future to replace
//optimal replacement policy
int opt(){
	int num_faults = 0;
	int num_refs = 0;

	vector<int> opt_frames;

	for (int k = 0; k < pages.size(); k++){
		bool is_fault = false;
		int i = pages[k];
		int pos = contains(opt_frames, i, 0);
		//did not find page
		if (pos == -1){
			//page fault, replace
			if (opt_frames.size() >= frameNum){
				//each page in frames is indexed with -1 so we can see if any are never used again
				vector<int> page_index;
				for (int j = 0; j < frameNum; j++){
					page_index.push_back(-1);
				}

				//finds the position in which each frame's page appears that's closest in future time to the current page frame
				for (int j = 0; j < opt_frames.size(); j++){
					if ((pos = contains(pages, opt_frames[j], k)) != -1){
						page_index[j] = pages.size() - 1 - pos;
					}
				}

				//finds the position that's not used for the longest time and replaces it
				int to_replace;
				int to_re_index = -2;
				for (int x = 0; x < page_index.size(); x++){
					if (to_re_index == -2){
						to_replace = opt_frames[x];
						to_re_index = page_index[x];
					}
					else if (page_index[x] < to_re_index){
						to_replace = opt_frames[x];
						to_re_index = page_index[x];
					}
				}

				pos = contains(opt_frames, to_replace, 0);
				opt_frames[pos] = i;

				is_fault = true;
				num_faults++;
				num_refs++;
			}
			//continue filling frames
			else{
				opt_frames.push_back(i);
			}
		}
		//if opt frames contains the page
		else{
			pos = contains(opt_frames, i, 0);
			if (opt_frames.size() >= frameNum) num_refs++;
		}
		cout << i << ": " << opt_frames;
		if (is_fault) cout << "  F";
		cout << endl;
	}

	cout << "Miss rate = ";
	if (num_refs == 0) cout << "N/A" << endl;
	else cout << num_faults << " / " << num_refs << " = " << (num_faults / (double)num_refs) * 100 << "%" << endl;

	return 0;
}

//clock replacement policy
void clock_cache() {
	vector<int> clock_frames;
	vector<int> usage;
	int i = 0;
	int num_faults = 0;
	int num_refs = 0;
	for (vector<int>::iterator it = pages.begin(); it != pages.end(); it++) {
		vector<int>::iterator inCacheIterator = find(clock_frames.begin(), clock_frames.end(), *it);
		bool is_fault = false;
		//did not find page
		if (inCacheIterator == clock_frames.end()) { 
			//continue filling frames
			if (clock_frames.size() < frameNum) {
				clock_frames.push_back(*it);
				usage.push_back(1);
			}
			//page fault, replace
			else { 
				is_fault = true;
				num_faults++;
				num_refs++;
				while (true) {
					//found a page to replace
					if (usage[i] == 0) {
						clock_frames[i] = *it;
						usage[i] = 1;
						i = (i + 1) % frameNum;
						break;
					}
					else {
						usage[i] = 0;
					}
					//makes sure i is never longer than frameNum
					i = (i + 1) % frameNum;
				}
			}
		}
		//if clock frames contains the page
		else {
			int indexInCache;
			for (indexInCache = 0; clock_frames[indexInCache] != *inCacheIterator; indexInCache++);
			usage[indexInCache] = 1;
			if (clock_frames.size() >= frameNum) num_refs++;
		}
		cout << i << ": " << clock_frames;
		if (is_fault) cout << "  F";
		cout << endl;
	}

	cout << "Miss rate = ";
	if (num_refs == 0) cout << "N/A" << endl;
	else cout << num_faults << " / " << num_refs << " = " << (num_faults / (double)num_refs) * 100 << "%" << endl;
}

//reads in numbers from file and pushes them into a vector
int readFile(string input)
{
	ifstream ifs(input);
	if (ifs.is_open())
	{
		string page;
		while (ifs >> page)
		{
			pages.push_back(stoi(page));
		}
	}
	else {
		cout << "Cannot find file" << endl;
		return 1;
	}

	return 0;
}
int usage(){
	cout << "usage: vmsim <num_physical_mem_frames> <input_file> <chosen_alg> " << endl;
	cout << "\tchosen_alg: FIFO, LRU, OPT, CLOCK" << endl;
	return 0;
}

int main(int argc, char* argv[])
{
	//checks for 4 inputs (total number of physical memory frames, input file, chosen algorithm)
	if (argc != 4) {
		//prints usage if you give the wrong number of arguments
		usage();
			//returns number thats not 1 to reflect not a success
		return 1;
	}
	else {
		//checks frameNum is a number max 100
		try {
			frameNum = stoi(argv[1]);
		}
		catch (...){
			cout << "Number of frames not a number" << endl;
			usage();

			return 1;
		}
		if (frameNum > 100){
			cout << "Number of frames too large" << endl;
			usage();

			return 1;
		}
		
		string fileName = argv[2];
		string alg = argv[3];
		if (!readFile(fileName)){
			if (alg == "LRU") lru();
			else if (alg == "FIFO") fifo();
			else if (alg == "OPT") opt();
			else if (alg == "CLOCK") clock_cache();
			else usage();

		}
		else usage();
	}
	return 0;
}
