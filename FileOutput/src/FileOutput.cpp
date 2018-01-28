#include<iostream>
#include<sstream>
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<time.h>
#include<string>
#include<fstream>
#include<fcntl.h>
#include <sys/types.h> 
#include <sys/stat.h>
#include <errno.h>
using namespace std;

enum Coding_Technique {Reed_Sol_Van, Reed_Sol_R6_Op, Cauchy_Orig, Cauchy_Good, Liberation, Blaum_Roth, Liber8tion, RDP, EVENODD, No_Coding};

string getElemTypeName(enum Coding_Technique tech){
	switch(tech){
		case Reed_Sol_Van:return "Reed_Sol_Van";break;
		case Reed_Sol_R6_Op:return "Reed_Sol_R6_Op";break;
		case Cauchy_Orig:return "Cauchy_Orig";break;
		case Cauchy_Good:return "Cauchy_Good";break;
		case Liberation:return "Liberation";break;
		case Blaum_Roth:return "Blaum_Roth";break;
		case Liber8tion:return "Liber8tion";break;
		case RDP:return "RDP";break;
		case EVENODD:return "EVENODD";break;
		case No_Coding:return "No_Coding";break;
	}
}

void InitOutputFile(const char* path, string k, string m, string w, string failed_disk_id, enum Coding_Technique tech, string algorithm){
	int fd = -1;
	// now time

	time_t Now;
	Now = time(&Now);

	if(access(path, F_OK) == -1){
		fd = open(path, O_RDWR | O_CREAT, 0664);
		if (fd == -1) {
			fprintf(stderr,  "Unable to open file.\n");
			exit(0);
		}
		string base = "COMMENT : The search result by ";
		string buf = base + algorithm + " for " + getElemTypeName(tech) +" "+ ctime(&Now) +"\n";
		buf += "tech : " + getElemTypeName(tech) + "\n";
		buf += "algorithm : " + algorithm + "\n";
		buf += "k : " + k + "\n";
		buf += "m : " + m + "\n";
		buf += "w : " + w + "\n";
		buf += "failed_disk_id : "+ failed_disk_id+"\n";
		buf += "conventional_symbol_number    optimized_symbol_number    reduced_symbol_number    time\n";		

		write(fd, buf.c_str(), buf.size());
		if(fd != -1)
			close(fd);
	}
}

int WriteResult(int conventional_symbol_number, int optimized_symbol_number, int reduced_symbol_number, int k, int m, int w, int failed_disk_id, enum Coding_Technique tech, string algorithm, double cost){
	int fd = -1;	
	const char* path;
	time_t Now;
	Now = time(&Now);
	//int to string
	stringstream stream_k, stream_m, stream_w, stream_failed_disk_id_str;
	string k_str, m_str, w_str, failed_disk_id_str;
	
	stream_k << k;
	stream_k >> k_str;
	stream_k.clear();

	stream_m << m;
	stream_m >> m_str;
	stream_m.clear();

	stream_w << w;
	stream_w >> w_str;
	stream_w.clear();

	stream_failed_disk_id_str << failed_disk_id;
	stream_failed_disk_id_str >> failed_disk_id_str;
	stream_failed_disk_id_str.clear();

	switch(tech){
		case Cauchy_Orig:
			{
				string base = "../Result/";
				base += algorithm + "/Cauchy_Orig-" + k_str + "-" + m_str + "-" + w_str + "-" + failed_disk_id_str;
				path = base.c_str();
				InitOutputFile(path, k_str, m_str, w_str, failed_disk_id_str, tech, algorithm);
				fd = open(path, O_RDWR | O_APPEND);
				break;
			}
		case Cauchy_Good:
			{
				string base = "../Result/";
				base += algorithm + "/Cauchy_Good-" + k_str + "-" + m_str + "-" + w_str + "-" + failed_disk_id_str;
				path = base.c_str();
				InitOutputFile(path, k_str, m_str, w_str, failed_disk_id_str, tech, algorithm);
				fd = open(path, O_RDWR | O_APPEND);
				break;
			}
		case Liberation:
			{
				string base = "../Result/";
				base += algorithm + "/Liberation-" + k_str + "-" + m_str + "-" + w_str + "-" + failed_disk_id_str;
				path = base.c_str();
				InitOutputFile(path, k_str, m_str, w_str, failed_disk_id_str, tech, algorithm);
				fd = open(path, O_RDWR | O_APPEND);
				break;
			}
		case Blaum_Roth:
			{
				string base = "../Result/";
				base += algorithm + "/Blaum_Roth-" + k_str + "-" + m_str + "-" + w_str + "-" + failed_disk_id_str;
				path = base.c_str();
				InitOutputFile(path, k_str, m_str, w_str, failed_disk_id_str, tech, algorithm);
				fd = open(path, O_RDWR | O_APPEND);
				break;
			}
		case Liber8tion:
			{
				string base = "../Result/";
				base += algorithm + "/Liber8tion-" + k_str + "-" + m_str + "-" + w_str + "-" + failed_disk_id_str;
				path = base.c_str();
				InitOutputFile(path, k_str, m_str, w_str, failed_disk_id_str, tech, algorithm);
				fd = open(path, O_RDWR | O_APPEND);
				break;
			}
	}
	if (fd == -1) {
		fprintf(stderr,  "Unable to open file.\n");
		exit(0);
	}
	//int to string

	stringstream stream_conventional_symbol_number, stream_optimized_symbol_number, stream_reduced_symbol_number, stream_time;
	string conventional_symbol_number_str, optimized_symbol_number_str, reduced_symbol_number_str, time;
		
	stream_conventional_symbol_number << conventional_symbol_number;
	stream_conventional_symbol_number >> conventional_symbol_number_str;
	stream_conventional_symbol_number.clear();
	
	stream_optimized_symbol_number << optimized_symbol_number;
	stream_optimized_symbol_number >> optimized_symbol_number_str;
	stream_optimized_symbol_number.clear();

	stream_reduced_symbol_number << reduced_symbol_number;
	stream_reduced_symbol_number >> reduced_symbol_number_str;	
	stream_reduced_symbol_number.clear();

	stream_time << cost;
	stream_time >> time;
	stream_time.clear();
	string buf = conventional_symbol_number_str + ",                            " + optimized_symbol_number_str +",                       " + reduced_symbol_number_str + ",                       " + time;
	write(fd, buf.c_str(), buf.size());
	if(fd != -1)
		close(fd);
	return 0;
}
