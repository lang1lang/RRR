#include<vector>
#include<map>
#include<iostream>
using namespace std;
int* sa_crs_hybrid_recovery_solution(int m, int k, int w, int failed_disk_id, int *generator_matrix);
void sa_crs_print_parity_group_selection(int m, int w, int* sa_crs_hybrid_parity_group_selection);
int sa_calculate_block_saving(int k, int m, int w, int failed_disk_id, int *generator_matrix, int* sa_crs_hybrid_parity_group_selection);
void sa_get_recovery_solution(int k, int m, int w, int failed_disk_id, int* generator_matrix, int* sa_crs_parity_group_selection, map<int, vector<int> >& crs_solution);
void sa_print_crs_solution(int k, int m, int w, map<int, vector<int> > crs_solution);