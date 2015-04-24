#include <iostream>
#include <fstream>
#include <sstream>
#include <stdlib.h>
#include <string>
#include <vector>
#include <time.h>
#define LARGE_REF 1000
#define MED_REF 100
#define SMALL_REF 10

using namespace std;

int main(int argc, char* argv[]){
	if(argc != 4){
		cout << "USAGE: ./generate_input <L/M/S>  <MP [many processes]/LP [large proceses]> <LRU/OTHER> " << endl;
		return 0;	
	}
	srand(time(NULL));
	int references = 0;
	string flag(argv[1]);
	if(flag == "L"){
		references = LARGE_REF;
	}
	else if(flag == "M"){
		references = MED_REF;
	}
	else if(flag == "S"){
		references = SMALL_REF;
	}
	else{
		references = MED_REF;
	}
	string process_type(argv[2]);
	string mode(argv[3]);
	vector<vector<int>> process_list;
	vector<bool> process_started;
	int num_processes = 0;
	int num_pages = 0;
	if(process_type == "MP"){
		cout << "Generating many small processes with around " << references << " memory references" << endl;
		num_processes = rand() % 30 + 70;
		for(int i = 0; i < num_processes; i++){
			num_pages = rand() % 10;
			vector<int> process_array; 
			for(int j = 1; j < num_pages + 2; j++){
				process_array.push_back(j);
			}
			process_list.push_back(process_array);
			process_started.push_back(false);
		}
	}
	else{
		cout << "Generating few large processes with around " << references << " memory references" << endl;
		num_processes = rand() % 10 + 1;
		for(int i = 0; i < num_processes; i++){
			num_pages = rand() % 200;
			vector<int> process_array;
			for(int j = 1; j < num_pages + 1; j++){
				process_array.push_back(j);
			}
			process_list.push_back(process_array);
			process_started.push_back(false);
		}
	}
	int ref_left = rand() % references + (references/5);
 	ofstream outfile;
	outfile.open("generated_output.txt");
	if(mode == "LRU"){
		while(ref_left){
			int process = rand() % num_processes;
			if(process_started[process] == false){
				outfile << "START " << process << " " << process_list[process].size() << endl;
				process_started[process] = true;
			}
			else{
				int burst_amount = 16;
				// Getting an arithmetic exception at next line which could only be caused if the process_list[process].size() is 0
				int rand_page_start = rand() % process_list[process].size();
				for(int i = 0; i < burst_amount; i++){
					int rand_page = rand_page_start + i;
					rand_page = rand_page % process_list[process].size();
					outfile << "REFERENCE " << process << " " << process_list[process][rand_page] << endl;
					ref_left--;
					if(ref_left == 0){
						break;
					}
					//reference here to keep page resident in memory
					outfile << "REFERENCE " << process << " " << process_list[process][rand_page_start] << endl;
				}
				//simulate many references to highly used location
				for(int j = 0; j < 16; j++){
					outfile << "REFERENCE " << process << " " << process_list[process][rand_page_start] << endl;
				}	       
			}
			int chance_to_terminate = rand() % 100;
			if(chance_to_terminate < 33){ // NOTE: This has a 1/100 chance of happening
				outfile << "TERMINATE " << process <<  endl;
				process_started[process] = false;
			}
		}
	}
	else{
		while(ref_left){
			int process = rand() % num_processes;
			if(process_started[process] == false){
				outfile << "START " << process << " " << process_list[process].size() << endl;
				process_started[process] = true;
			}
			else{
				int burst_amount = rand() % 4;
				for(int i = 1; i < burst_amount + 1; i++){
					int rand_page = rand() % process_list[process].size();
					outfile << "REFERENCE " << process << " " << process_list[process][rand_page] << endl;
					ref_left--;
					if(ref_left == 0){
						break;
					}
				}
			}
			int chance_to_terminate = rand() % 100;
			if(chance_to_terminate == 33){
				outfile << "TERMINATE " << process <<  endl;
				process_started[process] = false;
			}
		}
		
	}
	outfile.close();
	return 0;
}	

