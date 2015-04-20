#include "VirtualMemorySim.h"
using namespace std;

VirtualMemorySimulator::VirtualMemorySimulator(int num_frames, int mode)
{
	//initialize physical memory to -1, representing vacant
	//physical_memory size represents
	for(int i = 0; i < num_frames; i++){
		//physical_memory.push_back(pair<int, int>(-1,-1));

		struct frame f(false, FREE_FRAME, 0);
		physical_memory.push_back(f);
	}
	replacement_policy  = mode;
	fault_count = 0;
}

void VirtualMemorySimulator::start(int process_id, int size){
	struct process p1;
	//p1.pid = process_id; //this may be unnecessary
	p1.num_pages = size;
	for(int i = 0; i < p1.num_pages; i++){
		//vector pages represents a processes address space
		//each index represents a page
		//a value of -1 represents a null reference
		p1.pages.push_back(-1);
	}
	pair<int, struct process> pair1(process_id, p1);
	virtual_memory.insert(pair1);
}

// Only used for the LRU replacement policy
// This function increments the ages of all frames in physical memory
// except for the page number passed in as an argument
void VirtualMemorySimulator::incAllOtherAges(int page_number){
	for(int page = 0; page < physical_memory.size(); page++){
		if(page != page_number){
			physical_memory[page].age++;
		}
	}	
}
void VirtualMemorySimulator::fifoReplacement(int pid, int page_number){
	if(fifo.size() != physical_memory.size()){
		cout << "ERROR: Incoherence between queue and physical memory" << endl;
	}
	pair<int, struct frame> element = fifo.front();
	fifo.pop();
	int location = element.first;
	struct frame target = element.second;
	for(int i = 0; i < virtual_memory[target.pid].num_pages; i++){
		if(virtual_memory[target.pid].pages[i] == location){
		       virtual_memory[target.pid].pages[i] = -1;
		}
 	}
	physical_memory[location].pid = pid;
       	physical_memory[location].valid = false;
	
	virtual_memory[pid].pages[page_number] = location;	
}
int VirtualMemorySimulator::reference(int pid, int page_number){
	// Check if the page is not resident
	bool resident = false;
	struct process *p = &virtual_memory[pid];
	if(p->pages[page_number] != -1){
		resident = true;
	}

	// If the page number is not resident, check for available memory
	if(!resident){
		fault_count++;

		for(int page = 0; page < physical_memory.size(); page++){
			if(!physical_memory[page].valid){
				// Found a free frame of memory, allocate it
				p->pages[page_number] = page;
				physical_memory[page].valid = false;
				physical_memory[page].pid = pid;

				physical_memory[page].age = 0;
				incAllOtherAges(page);
				// Do nothing, age is not used
				// Set age to the current time
				physical_memory[page].age = clock();
				//place filled frame onto FIFO queue
				fifo.push(pair<int, struct frame>(page, physical_memory[page]));
				return PAGE_SUCCESS;
			}
		}

		// If we get to this point, there are no frames available in physical memory
		// Implement replacement policies here --------------------------------
		if(replacement_policy = MODE_RANDOM){
			//call random replacement
		}
		else if(replacement_policy = MODE_LRU){
			//call LRU replacement
		}
		else{
			fifoReplacement(pid, page_number);
		}

		// --------------------------------------------------------------------

		return PAGE_FAULT;
	}
	else{
		physical_memory[page_number].age = 0;
		incAllOtherAges(page_number);
		return PAGE_RESIDENT;
	}
}

void VirtualMemorySimulator::terminate(int process_id){
	// Free all frames the process references
	//for(auto f : physical_memory){
	//	if(f.pid == process_id){
	//		// Deallocate the frame
	//		f.valid = false;
	//		f.pid = -1;
	//		f.age = 0;
	//	}
	//}

	// Remove the process from the map
	struct process p = virtual_memory[process_id];
	if(replacement_policy == MODE_QUEUE){
		int count = 0;
		int size = fifo.size();
		while(count < size){
			pair<int, struct frame> p1 = fifo.front();
			fifo.pop();
			if(p1.second.pid != process_id){
				fifo.push(p1);
			}
			count++;
		}
	}
	for(int i = 0; i < p.num_pages; i++){
		if(p.pages[i] != -1){
			physical_memory[i].valid = false;
			physical_memory[i].pid = FREE_FRAME;
			physical_memory[i].age = 0;
		}
	}
	virtual_memory.erase(process_id);
}

