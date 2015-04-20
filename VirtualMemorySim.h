#include <string>
#include <vector>
#include <utility>
#include <math.h>
#include <ctime>
#include <map>
#include <queue>
#include <iostream>

#define FREE_FRAME -1

#define PAGE_FAULT 0
#define PAGE_SUCCESS 1
#define PAGE_RESIDENT 2

#define MODE_RANDOM 1
#define MODE_LRU 2
#define MODE_QUEUE 3

class VirtualMemorySimulator{
	private:
		int replacement_policy;		
		int fault_count;
		
		struct process{
		//	int pid;
			int num_pages;
			std::vector<int> pages;
		};

		// This struct will contain info such as age for LRU, size if needed
		// and everything else for the page replacement algorithms
		// frame.pid will be -1 for unallocated frames
		struct frame{
			bool valid;
			int pid; // The process the frame is allocated to
			clock_t age; // LRU: Used to monitor access times ; FIFO: Used to store allocation time
			frame(bool v, int p, clock_t a)
			: valid(v), pid(p), age(a){}
		};

		// The vector of frame structs representing physical memory
		std::vector<frame> physical_memory;

		// Format for physical_memory is pair<pid, page>
		//std::vector< std::pair<int, int>> physical_memory;

		// pair format is <pid, process>
		std::map<int,struct process> virtual_memory;
		std::queue<std::pair<int, struct frame>> fifo;
	public:
		VirtualMemorySimulator(int num_framesi, int mode);
		void start(int pid, int size);
		void terminate(int pid);
		int reference(int pid, int page_number);
		void incAllOtherAges(int page_number);
		void fifoReplacement(int pid, int page_number);
		void randomReplacement(int pid, int page_number);
		void LRUReplacement(int pid, int page_number);
		void replaceFrameHolder(int frame, int pid, int page_number);
};
