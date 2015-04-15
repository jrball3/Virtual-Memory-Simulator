#include "VirtualMemorySimulator.h"
using namespace std;

VirtualMemorySimulator::VirtualMemorySimulator(int num_frames, int replacement_policy)
: mode(replacement_policy) {
	//initialize physical memory to -1, representing vacant
	//physical_memory size represents
	for(int i = 0; i < num_frames; i++){
		//physical_memory.push_back(pair<int, int>(-1,-1));

		struct frame f(false, 0, 0);
		physical_memory.push_back(f);
	}
}

void VirtualMemorySimulator::start(int process_id, int size){
	struct process p1;
	//p1.pid = process_id; //this may be unnecessary
	p1.num_pages = size;
	for(int i = 0; i < num_pages; i++){
		//vector pages represents a processes address space
		//each index represents a page
		//a value of -1 represents a null reference
		p1.pages.push_back(-1);
	}
	virtual_memory.push_back(pair<process_id, p1>);
}

int VirtualMemorySimulator::reference(int pid, int page_number){
	// Check if the page is not resident
	bool resident = false;
	struct process *p = &virtual_memory[pid];
	for(auto it : p.pages){
		if(it == page_number){
			resident = true;
		}
	}

	// If the page number is not resident, check for available memory
	if(!resident){
		for(int page = 0; page < physical_memory.size(); page++){
			if(!physical_memory[page].valid){
				// Found a free frame of memory, allocate it
				p->pages.push_back(page);
				physical_memory[page].valid = true;
				physical_memory[page].pid = pid;

				if(mode == MODE_LRU){
					physical_memory[page].age = 0;
					incAllOtherAges(page);
				}
				else if(mode == MODE_RANDOM){
					// Do nothing, age is not used
				}
				else if(mode == MODE_FIFO){
					// Set age to the current time
					physical_memory[page].age = clock();
				}

				return PAGE_SUCCESS;
			}
		}

		// If we get to this point, there are no frames available in physical memory
		// Implement replacement policies here --------------------------------



		// --------------------------------------------------------------------

		return PAGE_FAULT;
	}
	else{
		// Page is resident, successful reference
		if(mode == MODE_LRU){
			physical_memory[page_number].age = 0;
			incAllOtherAges(page_number);
		}
		else if(mode == MODE_RANDOM){
			// Do nothing, age is not used
		}
		else if(mode == MODE_FIFO){
			// Do nothing, age is not updated on each access
		}
		return PAGE_RESIDENT;
	}
}

// Only used for the LRU replacement policy
// This function increments the ages of all frames in physical memory
// except for the page number passed in as an argument
void incAllOtherAges(int page_number){
	for(int page = 0; page < physical_memory.size(); page++){
		if(page != page_number){
			physical_memory[page].age++;
		}
	}	
}


void VirtualMemorySimulator::terminate(int process_id){
	// Free all frames the process references
	for(auto f : physical_memory){
		if(f.pid == process_id){
			// Deallocate the frame
			f.valid = false;
			f.pid = 0;
			f.age = 0;
		}
	}

	// Remove the process from the map
	auto p = virtual_memory.find(process_id);
	if(p != virtual_memory.end())
		virtual_memory.erase(p);
}