#include<stdio.h>
#include<stdlib.h>
#include<iostream>
#include<vector>
#include<map>
#include<string.h>
#include<time.h>
#include "cauchy.h" 
#include "jerasure.h"
#include "liberation.h"
#include "enumeration_search.h"
#include "FileOutput.h"
using namespace std;
bool is_prime(int N) {
	if (N < 2) {
		return true;
	}
	for (int i = 2; i*i < N; ++i) {
		if (N % i == 0) {
			return false;
		}
	}
	return true;
}
double GetTime()
{
    return (double) clock() / CLOCKS_PER_SEC;
}
int main(int argc, char **argv) {
	int *matrix = NULL;
	int *generator_matrix = NULL;
	int **schedule = NULL;
	enum Coding_Technique tech;	// coding technique (parameter)
	int k, m, w, failed_disk_id;
	double LastTime, Time;
	string algorithm = "Enumeration";

	if(argc != 6){
		fprintf(stderr, "usage: coding_technique k m w failed_disk_id\n");
		fprintf(stderr,  "\nChoose one of the following coding techniques: \ncauchy_orig, \ncauchy_good, \nliberation, \nblaum_roth, \nliber8tion\n\n");
		exit(0);
	}
	
	/* Conversion of parameters and error checking */	
	if (sscanf(argv[2], "%d", &k) == 0 || k <= 0) {
		fprintf(stderr,  "Invalid value for k\n");
		exit(0);
	}
	if (sscanf(argv[3], "%d", &m) == 0 || m < 0) {
		fprintf(stderr,  "Invalid value for m\n");
		exit(0);
	}
	if (sscanf(argv[4],"%d", &w) == 0 || w <= 0) {
		fprintf(stderr,  "Invalid value for w.\n");
		exit(0);
	}
	if (sscanf(argv[5], "%d", &failed_disk_id) == 0 || failed_disk_id < 0){
		fprintf(stderr,  "Invalid value for failed_disk_id.\n");
                exit(0);
	}

	/* Setting of coding technique and error checking */
	if (strcmp(argv[1], "cauchy_orig") == 0) {
		tech = Cauchy_Orig;
	}
	else if (strcmp(argv[1], "cauchy_good") == 0){
		tech = Cauchy_Good;
	}
	else if (strcmp(argv[1], "liberation") == 0){
		if (k > w) {
			fprintf(stderr,  "k must be less than or equal to w\n");
			exit(0);
		}
		if (w <= 2 || !(w%2) || !is_prime(w)) {
			fprintf(stderr,  "w must be greater than two and w must be prime\n");
			exit(0);
		}
		tech = Liberation;
	}
	else if (strcmp(argv[1], "blaum_roth") == 0) {
		if (k > w) {
			fprintf(stderr,  "k must be less than or equal to w\n");
			exit(0);
		}
		if (w <= 2 || !((w+1)%2) || !is_prime(w+1)) {
			fprintf(stderr,  "w must be greater than two and w+1 must be prime\n");
			exit(0);
		}
		tech = Blaum_Roth;
	}
	else if (strcmp(argv[1], "liber8tion") == 0){
		if (w != 8) {
			fprintf(stderr, "w must equal 8\n");
			exit(0);
		}
		if (m != 2) {
			fprintf(stderr, "m must equal 2\n");
			exit(0);
		}
		if (k > w) {
			fprintf(stderr, "k must be less than or equal to w\n");
			exit(0);
		}
		tech = Liber8tion;
	}
	else {
		fprintf(stderr,  "Not a valid coding technique. Choose one of the following: cauchy_orig, cauchy_good, liberation, blaum_roth, liber8tion\n");
		exit(0);
	}

	switch (tech) {
	case Cauchy_Orig:	
		matrix = cauchy_original_coding_matrix(k, m, w);
		generator_matrix = jerasure_matrix_to_bitmatrix(k, m, w, matrix);
		schedule = jerasure_smart_bitmatrix_to_schedule(k, m, w, generator_matrix);
		break;
	case Cauchy_Good:
		matrix = cauchy_good_general_coding_matrix(k, m, w);
		generator_matrix = jerasure_matrix_to_bitmatrix(k, m, w, matrix);
		schedule = jerasure_smart_bitmatrix_to_schedule(k, m, w, generator_matrix);
		break;
	case Liberation:
		generator_matrix = liberation_coding_bitmatrix(k, w);
		schedule = jerasure_smart_bitmatrix_to_schedule(k, m, w, generator_matrix);
		break;
	case Blaum_Roth:
		generator_matrix = blaum_roth_coding_bitmatrix(k, w);
		schedule = jerasure_smart_bitmatrix_to_schedule(k, m, w, generator_matrix);
		break;
	case Liber8tion:
		generator_matrix = liber8tion_coding_bitmatrix(k);
		schedule = jerasure_smart_bitmatrix_to_schedule(k, m, w, generator_matrix);
		break;
	}

	//cout << "Output generation matrix: " << endl;
	//jerasure_print_bitmatrix(generator_matrix, m*w, k*w, w);

	cout << "Output the matrix search results of enumeration algorithm: " << endl;
	LastTime = GetTime();
	enumeration_crs_hybrid_recovery_solution(k, m, w, failed_disk_id, generator_matrix);
	Time = GetTime() - LastTime;
	
	int block_saving = enumeration_block_saving(k, m, w, failed_disk_id);
	int conventional_symbol_number = (k - 1)*w + w;
	int optimized_symbol_number    = (k - 1)*w + m*w- block_saving;
	cout << "Conventional recovery solution: " << conventional_symbol_number << "   ";
	cout << "Enumeration recovery solution: " << optimized_symbol_number << endl;
	cout << "Reduction amount:  " << conventional_symbol_number - optimized_symbol_number << endl;

	WriteResult(conventional_symbol_number, optimized_symbol_number, block_saving, k, m, w, failed_disk_id, tech, algorithm, Time);
	cout<<"Time: "<< Time <<endl;
	return 0;
}
