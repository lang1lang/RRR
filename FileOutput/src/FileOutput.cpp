#include<iostream>
#include<sstream>
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<time.h>
#include<fstream>
using namespace std;

enum Coding_Technique {Reed_Sol_Van, Reed_Sol_R6_Op, Cauchy_Orig, Cauchy_Good, Liberation, Blaum_Roth, Liber8tion, RDP, EVENODD, No_Coding};

int WriteResult(int conventional_symbol_number, int optimized_symbol_number, int reduced_symbol_number, int k, int m, int w, int failed_disk_id, enum Coding_Technique tech, string algorithm, double cost){
	FILE* fp;
	const char* path;
	time_t Now;

	//int to string
	stringstream kk,mm,ww,failed_node_id;
	kk<<k;
	mm<<m;
	ww<<w;
	failed_node_id<<failed_disk_id;
	// now time

	Now = time(&Now);
	switch(tech){
		case Cauchy_Orig:
			{
				string base = "..\\Result\\Cauchy_Orig-";
				path = (base + kk.str() + "-" + mm.str() + "-" + ww.str() + "-" + failed_node_id.str()).c_str();
				if(access(path,F_OK)==-1){
					ofstream fout(path);                                      //Create output file.
					if(!fout){
						fprintf(stderr,  "Unable to create file.\n");
					}
					fp = fopen(path,"wb");
					fprintf(fp, "COMMENT : The search result by %s for %d %s\n", algorithm.c_str(), tech, ctime(&Now));
					fclose(fp);
					
				}
				fp = fopen(path, "wb");
				break;
			}
		case Cauchy_Good:
			{
				string base = "..\\Result\\Cauchy_Good-";
				path = (base + kk.str() + "-" + mm.str() + "-" + ww.str() + "-" + failed_node_id.str()).c_str();
				if(access(path,F_OK)==-1){
					ofstream fout(path);
					if(!fout){
						fprintf(stderr,  "Unable to create file.\n");
					}
					fp = fopen(path, "wb");
					fprintf(fp, "COMMENT : The search result by %s for %d %s\n", algorithm.c_str(), tech, ctime(&Now));
					fclose(fp);
				}
				fp = fopen(path, "wb");
				break;
			}
		case Liberation:
			{
				string base = "..\\Result\\Liberation";
				path = (base + kk.str() + "-" + mm.str() + "-" + ww.str() + "-" + failed_node_id.str()).c_str();
				if(access(path,F_OK)==-1){
					ofstream fout(path);
					if(!fout){
						fprintf(stderr,  "Unable to create file.\n");
					}
					fp = fopen(path,"wb");
					fprintf(fp, "COMMENT : The search result by %s for %d %s\n", algorithm.c_str(), tech, ctime(&Now));
					fclose(fp);
				}
				fp = fopen(path, "wb");
				break;
			}
		case Blaum_Roth:
			{
				string base = "..\\Result\\Blaum_Roth";
				path = (base + kk.str() + "-" + mm.str() + "-" + ww.str() + "-" + failed_node_id.str()).c_str();
				if(access(path,F_OK)==-1){
					ofstream fout(path);
					if(!fout){
						fprintf(stderr,  "Unable to create file.\n");
					}
					fp = fopen(path, "wb");
					fprintf(fp, "COMMENT : The search result by %s for %d %s\n", algorithm.c_str(), tech, ctime(&Now));
					fclose(fp);
				}
				fp = fopen(path, "wb");
				break;
			}
		case Liber8tion:
			{
				string base = "..\\Result\\Liber8tion";
				path = (base + kk.str() + "-" + mm.str() + "-" + ww.str() + "-" + failed_node_id.str()).c_str();
				if(access(path,F_OK)==-1){
					ofstream fout(path);
					if(!fout){
						fprintf(stderr,  "Unable to create file.\n");
					}
					fp = fopen(path, "wb");
					fprintf(fp, "COMMENT : The search result by %s for %d %s\n", algorithm.c_str(), tech, ctime(&Now));
					fclose(fp);
				}
				fp = fopen(path, "wb");
				break;
			}
	}
	if (fp == NULL) {
		fprintf(stderr,  "Unable to open file.\n");
		exit(0);
	}

		
	return 0;
}