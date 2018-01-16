#include<stdio.h>
#include<iostream>
using namespace std;

enum Coding_Technique {Reed_Sol_Van, Reed_Sol_R6_Op, Cauchy_Orig, Cauchy_Good, Liberation, Blaum_Roth, Liber8tion, RDP, EVENODD, No_Coding};
int WriteResult(int conventional_symbol_number, int optimized_symbol_number, int reduced_symbol_number, int k, int m, int w, int failed_disk_id, enum Coding_Technique tech, string algorithm, double cost);