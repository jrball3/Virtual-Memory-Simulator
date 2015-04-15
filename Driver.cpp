#include <fstream>
#include <stringstream>
#include <iostream>
#include <string>
#include "VirtualMemorySim.h"

using namespace std;

int main(int argc, char* argv[]){
	if(argc != 3){
		cout << "USAGE: ./program2 <frames-of-memory> <input-file>" << endl;
	}
	else{
		string filename(argv[2]);
		int num_frames = atoi(argv[1]);
		//to do: initialize number of frames of memory
		//do so using a class, which contains parameters representing frame size
		//as well as a data structure to store processes
		//processes should be indexed by process number
		VirtualMemorySim VMS(num_frames);
		ifstream infile(filename);
		if(infile.is_open()){
			string line, word;
			while(getline(infile, line)){
				stringstream ss(line);
				ss >> word;
				if(word == "START"){
					int process_number = 0;
					int address_space_size = 0;
					ss >> process_number;
					ss >> address_space_size;
					//call start proccess function
					VMS.start(process_number, address_space_size);
				}
				else if(word == "TERMINATE"){
					int process_number = 0;
					ss >> process_number;
					//call terminate process function
					VMS.terminate(process_number);
				}
				else{//REFERENCE
					int process_number = 0;
					int page_number = 0;
					ss >> process_number;
					ss >> page_number;
					//call reference function
					int result = VMS.reference(process_number, page_number);
					if(result == PAGE_FAULT){
						cout << "Encountered a page fault when attempting to add a reference
								from process id " << process_number << " to page number " << 
								page_number << endl;
					}
					else if(result == PAGE_SUCCESS){
						// Reference addition successful
					}
					else if(result == PAGE_RESIDENT){
						cout << "Page number " << page_number << " is already resident to
								process id " << process_number << endl;
					}
				}	
			}
		}
		infile.close();
	}
	return 0;
}
