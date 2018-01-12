#include<stdio.h>
#include<stdlib.h>
#include<iostream>
#include<vector>
#include<map>
#include<string>
#include<time.h>
#include "cauchy.h" 
#include "jerasure.h"
#include "liberation.h"
#include "sa_search.h"
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
int main() {
	int *matrix = NULL;
	int *generator_matrix = NULL;
	int **schedule = NULL;
	int k, m, w, failed_disk_id;
	cout << "Please enter the type of erasure code:" << endl;
	cout << "1:" << "Cauchy_Orig" << " 2:" << "Cauchy_Good" << " 3:" << "Liberation" << " 4:" << "Blaum_Roth" << " 5:" << "Liber8tion" << endl;
	int type = 0;
	cin >> type;
	switch (type) {
	case 1:
		cout << "please input k,m,w,failed_disk_id:" << endl;
		cin >> k >> m >> w >> failed_disk_id;
		matrix = cauchy_original_coding_matrix(k, m, w);
		generator_matrix = jerasure_matrix_to_bitmatrix(k, m, w, matrix);
		schedule = jerasure_smart_bitmatrix_to_schedule(k, m, w, generator_matrix);
		break;
	case 2:
		cout << "please input k,m,w,failed_disk_id:" << endl;
		cin >> k >> m >> w >> failed_disk_id;
		matrix = cauchy_good_general_coding_matrix(k, m, w);
		generator_matrix = jerasure_matrix_to_bitmatrix(k, m, w, matrix);
		schedule = jerasure_smart_bitmatrix_to_schedule(k, m, w, generator_matrix);
		break;
	case 3:
		cout << "please input k,m,w,failed_disk_id:" << endl;
		cin >> k >> m >> w >> failed_disk_id;
		if (k > w) {
			cout << "k must be less than or equal to w" << endl;
			system("pause");
			exit(0);
		}
		if (w <= 2 || !(w % 2) || !is_prime(w)) {
			cout << "w must be greater than two and w must be prime" << endl;
			system("pause");
			exit(0);
		}
		generator_matrix = liberation_coding_bitmatrix(k, w);
		schedule = jerasure_smart_bitmatrix_to_schedule(k, m, w, generator_matrix);
		break;
	case 4:
		cout << "please input k,m,w,failed_disk_id:" << endl;
		cin >> k >> m >> w >> failed_disk_id;
		if (k > w) {
			cout << "k must be less than or equal to w" << endl;
			system("pause");
			exit(0);
		}
		if (w <= 2 || !((w + 1) % 2) || !is_prime(w + 1)) {
			cout << "w must be greater than two and w+1 must be prime" << endl;
			system("pause");
			exit(0);
		}
		generator_matrix = blaum_roth_coding_bitmatrix(k, w);
		schedule = jerasure_smart_bitmatrix_to_schedule(k, m, w, generator_matrix);
		break;
	case 5:
		cout << "please input k,m,w,failed_disk_id:" << endl;
		cin >> k >> m >> w >> failed_disk_id;
		if (w != 8) {
			cout << "w must equal 8" << endl;
			system("pause");
			exit(0);
		}
		if (m != 2) {
			cout << "m must equal 2" << endl;
			system("pause");
			exit(0);
		}
		if (k > w) {
			cout << "k must be less than or equal to w" << endl;
			system("pause");
			exit(0);
		}
		generator_matrix = liber8tion_coding_bitmatrix(k);
		schedule = jerasure_smart_bitmatrix_to_schedule(k, m, w, generator_matrix);
		break;
	}
	cout << "Output generation matrix:" << endl;
	jerasure_print_bitmatrix(generator_matrix, m*w, k*w, w);
	
	cout << endl << endl << "Output the matrix search results of simulated annealing algorithm:" << endl;
	int max = 0;
	int* sa_crs_parity_group_selection = sa_crs_hybrid_recovery_solution(k, m, w, failed_disk_id, generator_matrix);
	sa_crs_print_parity_group_selection(m, w, sa_crs_parity_group_selection);
	int sa_block_saving = sa_calculate_block_saving(k, m, w, failed_disk_id, generator_matrix, sa_crs_parity_group_selection);
	cout << "Conventional recovery solution: " << (k - 1)*w +w<< "   ";
	cout << "Random recovery solution: " << (k - 1)*w - sa_block_saving +w << endl;
	cout << "Reduction amount:  " << sa_block_saving << endl;
	
	map<int, vector<int> > crs_solution;
	sa_get_recovery_solution(k,m,w, failed_disk_id, generator_matrix,sa_crs_parity_group_selection,crs_solution);
	cout << "Output the scheme for file reading and writing: " << endl;
	sa_print_crs_solution(k, m, w, crs_solution);
	return 0;
}
