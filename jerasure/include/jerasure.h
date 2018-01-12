#ifndef _JERASURE_H
#define _JERASURE_H
#include<vector>
#include<map>
#include<iostream>

using namespace std;
int *jerasure_matrix_to_bitmatrix(int k, int m, int w, int *matrix) ;
void jerasure_print_matrix(int *matrix, int rows, int cols, int w);
void jerasure_print_bitmatrix(int *m, int rows, int cols, int w);
void jerasure_matrix_encode(int k, int m, int w, int *matrix,char **data_ptrs, char **coding_ptrs, int size);
void jerasure_matrix_dotprod(int k, int w, int *matrix_row,int *src_ids, int dest_id,char **data_ptrs, char **coding_ptrs, int size);

int jerasure_matrix_decode(int k, int m, int w, int *matrix, int row_k_ones, int *erasures,char **data_ptrs, char **coding_ptrs, int size);
int *jerasure_erasures_to_erased(int k, int m, int *erasures);
int jerasure_invert_matrix(int *mat, int *inv, int rows, int w);
int jerasure_make_decoding_matrix(int k, int m, int w, int *matrix, int *erased, int *decoding_matrix, int *dm_ids);

int **jerasure_smart_bitmatrix_to_schedule(int k, int m, int w, int *bitmatrix);
void jerasure_schedule_encode(int k, int m, int w, int **schedule,char **data_ptrs, char **coding_ptrs, int size, int packetsize);
int jerasure_schedule_decode_lazy(int k, int m, int w, int *bitmatrix, int *erasures,char **data_ptrs, char **coding_ptrs, int size, int packetsize,int smart);
int jerasure_schedule_decode_lazy_hybrid_solution(int k, int m, int w, int *bitmatrix, int *erasures, char **data_ptrs, char **coding_ptrs, int size, int packetsize, int smart, map<int, vector<int> > crs_solution, int* crs_parity_group_selection);
void jerasure_fprint_bitmatrix(int *m, int rows, int cols, int w, FILE* outfile);
#endif