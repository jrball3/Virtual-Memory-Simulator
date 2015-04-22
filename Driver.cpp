//

#include <fstream>
#include <sstream>
#include <iostream>
#include <string>
#include "VirtualMemorySim.h"
#include <cstdlib>

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
		VirtualMemorySimulator VMS_LRU(num_frames, MODE_LRU);
		VirtualMemorySimulator VMS_QUEUE(num_frames, MODE_QUEUE);
		VirtualMemorySimulator VMS_RANDOM(num_frames, MODE_RANDOM);
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
					VMS_LRU.start(process_number, address_space_size);
					VMS_QUEUE.start(process_number, address_space_size);
					VMS_RANDOM.start(process_number, address_space_size);
				}
				else if(word == "TERMINATE"){
					int process_number = 0;
					ss >> process_number;
					//call terminate process function
					VMS_LRU.terminate(process_number);
					VMS_QUEUE.terminate(process_number);
					VMS_RANDOM.terminate(process_number);
				}
				else{//REFERENCE
					int process_number = 0;
					int page_number = 0;
					ss >> process_number;
					ss >> page_number;
					//call reference function
					int result = VMS_LRU.reference(process_number, page_number);
					if(result == PAGE_FAULT){
//						cout << "Encountered a page fault when attempting to add a reference from process id " << process_number << " to page number " << page_number << endl;
					}
					else if(result == PAGE_SUCCESS){
						// Reference addition successful
					}
					else if(result == PAGE_RESIDENT){
//						cout << "Page number " << page_number << " is already resident to process id " << process_number << endl;
					}
					
					int result2 = VMS_QUEUE.reference(process_number, page_number);	
					int result3 = VMS_RANDOM.reference(process_number, page_number);	
						
				}	
			}
		}
		infile.close();


		// Print fault counts
		cout << "Page Fault Rates: " << endl; 
		cout << "RANDOM: " << (VMS_RANDOM.getFaultCount() / VMS_RANDOM.getReferenceCount()) * 100 << "%" << endl;
		cout << "LRU: " << (VMS_LRU.getFaultCount() / VMS_LRU.getReferenceCount()) * 100 << "%" << endl;
		cout << "FIFO: " << (VMS_QUEUE.getFaultCount() / VMS_QUEUE.getReferenceCount()) * 100 << "%" << endl;
	}
	return 0;
}
