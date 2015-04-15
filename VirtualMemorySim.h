#include <string>
#include <vector>
#include <utility>
#include <math.h>

#define FREE_FRAME -1

#define PAGE_FAULT = 0;
#define PAGE_SUCCESS = 1;
#define PAGE_RESIDENT = 2;

#define MODE_RANDOM = 10;
#define MODE_LRU = 11;
#define MODE_FIFO = 12;


class VirtualMemorySimulator{
	private:
		int mode;
		
		struct process{
		//	int pid;
			int num_pages;
			std::vector<int> pages;
		};

		// This struct will contain info such as age for LRU, size if needed
		// and everything else for the page replacement algorithms
		// frame.pid will be -1 for unallocated frames
		struct frame{
			int pid; // The process the frame is allocated to
			int age; // LRU: Used to monitor access times ; FIFO: Used to store allocation time
			frame(int p)
			: pid(p){}
		};

		// The vector of frame structs representing physical memory
		std::vector<frame> physical_memory;

		// Format for physical_memory is pair<pid, page>
		//std::vector< std::pair<int, int>> physical_memory;

		// pair format is <pid, process>
		std::map<int,struct process> virtual_memory;

	public:
		VirtualMemorySimulator(int num_frames, int m);
		void start(int pid, int size);
		void terminate(int pid);
		int reference(int pid, int page_number);
		void incOtherAges(int page_number);
};
