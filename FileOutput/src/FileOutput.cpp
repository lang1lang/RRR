#include<iostream>
#include<sstream>
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<time.h>
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

void InitOutputFile(const char* path, string kk, string mm, string ww, string failed_node_id, enum Coding_Technique tech, string algorithm){
	FILE* fp;
	time_t Now;
	int fd;
	// now time
	Now = time(&Now);
	if(access(path, F_OK) == -1){
		fp = fopen(path, "a+");
		if(fp == NULL){
			fprintf(stderr, "Unable to open file.\n");
			exit(0);
		}
		fseek(fp, 0, SEEK_SET);
		fprintf(fp, "COMMENT : The search result by %s for %s %s\n", algorithm.c_str(), getElemTypeName(tech).c_str(), ctime(&Now));
		fprintf(fp, "tech : %s\n", getElemTypeName(tech).c_str());
		fprintf(fp, "algorithm : %s\n", algorithm.c_str());
		fprintf(fp, "k : %s\n", kk.c_str());
		fprintf(fp, "m : %s\n", mm.c_str());
		fprintf(fp, "w : %s\n", ww.c_str());
		fprintf(fp, "failed_disk_id : %s\n", failed_node_id.c_str());
		fprintf(fp, "conventional_symbol_number    optimized_symbol_number    reduced_symbol_number    time\n");
		fclose(fp);
	}
}

int WriteResult(int conventional_symbol_number, int optimized_symbol_number, int reduced_symbol_number, int k, int m, int w, int failed_disk_id, enum Coding_Technique tech, string algorithm, double cost){
	FILE* fp;
	const char* path;
	int fd;	
	time_t Now;
	Now = time(&Now);
	//int to string
	stringstream kk,mm,ww,failed_node_id;
	kk<<k;
	mm<<m;
	ww<<w;
	failed_node_id<<failed_disk_id;

	switch(tech){
		case Cauchy_Orig:
			{
				string base = "./Result/";
				base += algorithm + "/Cauchy_Orig-" + kk.str() + "-" + mm.str() + "-" + ww.str() + "-" + failed_node_id.str();
				path = base.c_str();
				InitOutputFile(path, kk.str(), mm.str(), ww.str(), failed_node_id.str(), tech, algorithm);
				fp = fopen(path, "a+");
				break;
			}
		case Cauchy_Good:
			{
				string base = "./Result/";
				base += algorithm + "/Cauchy_Good-" + kk.str() + "-" + mm.str() + "-" + ww.str() + "-" + failed_node_id.str();
				path = base.c_str();
				InitOutputFile(path, kk.str(), mm.str(), ww.str(), failed_node_id.str(), tech, algorithm);
				fp = fopen(path, "a+");
				break;
			}
		case Liberation:
			{
				string base = "./Result/";
				base += algorithm + "/Liberation-" + kk.str() + "-" + mm.str() + "-" + ww.str() + "-" + failed_node_id.str();
				path = base.c_str();
				InitOutputFile(path, kk.str(), mm.str(), ww.str(), failed_node_id.str(), tech, algorithm);
				fp = fopen(path, "a+");
				break;
			}
		case Blaum_Roth:
			{
				string base = "./Result/";
				base += algorithm + "/Blaum_Roth-" + kk.str() + "-" + mm.str() + "-" + ww.str() + "-" + failed_node_id.str();
				path = base.c_str();
				InitOutputFile(path, kk.str(), mm.str(), ww.str(), failed_node_id.str(), tech, algorithm);
				fp = fopen(path, "a+");
				break;
			}
		case Liber8tion:
			{
				string base = "./Result/";
				base += algorithm + "/Liber8tion-" + kk.str() + "-" + mm.str() + "-" + ww.str() + "-" + failed_node_id.str();
				path = base.c_str();
				InitOutputFile(path, kk.str(), mm.str(), ww.str(), failed_node_id.str(), tech, algorithm);
				fp = fopen(path, "a+");
				break;
			}
	}
	if (fp == NULL) {
		fprintf(stderr,  "Unable to open file.\n");
		exit(0);
	}
	fseek(fp, 0, SEEK_CUR);

	//int to string
	stringstream conventional_symbol_numbers, optimized_symbol_numbers, reduced_symbol_numbers, time;
	conventional_symbol_numbers << conventional_symbol_number;
	optimized_symbol_numbers << optimized_symbol_number;
	reduced_symbol_numbers << reduced_symbol_number;
	time<<cost;

	fprintf(fp, "%s,                            %s,                       %s,                       %s\n", conventional_symbol_numbers.str().c_str(), optimized_symbol_numbers.str().c_str(), reduced_symbol_numbers.str().c_str(), time.str().c_str());
	return 0;
}
