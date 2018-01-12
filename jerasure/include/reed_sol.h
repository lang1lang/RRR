#ifndef REED_SOL_H
#define  REED_SOL_H


int *reed_sol_vandermonde_coding_matrix(int k, int m, int w);
int *reed_sol_big_vandermonde_distribution_matrix(int rows, int cols, int w);
int *reed_sol_extended_vandermonde_matrix(int rows, int cols, int w);

#endif