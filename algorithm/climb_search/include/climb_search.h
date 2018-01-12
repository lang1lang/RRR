#include<vector>
#include<map>
#include<iostream>
using namespace std;
int* crs_hybrid_recovery_solution(int m, int k, int w, int failed_disk_id,int *generator_matrix);
void crs_print_parity_group_selection(int m, int k, int w, int failed_disk_id, int* crs_parity_group_selection);
void get_recovery_solution(int k, int m, int w, int failed_disk_id, int* generator_matrix,int* crs_parity_group_selection, map<int, vector<int> >& crs_solution);
void print_crs_solution(int k, int m, int w, map<int, vector<int> > crs_solution);
int calculate_block_saving(int k, int m, int w, int failed_disk_id, int *generator_matrix, int* sa_crs_hybrid_parity_group_selection);