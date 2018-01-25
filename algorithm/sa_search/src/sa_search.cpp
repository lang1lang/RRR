#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<iostream>
#include<string.h>
#include<cmath>
#include<vector>
#include<map>
#include"sa_search.h"
using namespace std;

int* sa_crs_hybrid_parity_group_selection;
int* sa_crs_hybrid_parity_group_selection_index;
int* sa_crs_hybrid_parity_group_selection_best;
int* sa_crs_hybrid_parity_group_selection_best_index;
int* sa_crs_hybrid_parity_group_selection_temporary;
int* sa_crs_hybrid_parity_group_selection_temporary_index;

int** sa_crs_recovery_equation_group;
int** sa_crs_recovery_equation_group_index;
int* sa_crs_recovery_equation_group_index_number;

int sa_crs_hybrid_profit;
			 
void init_crs_recovery_equation_group(int k,int m ,int w,int failed_disk_id,int *generator_matrix) {
	sa_crs_recovery_equation_group = new int*[w];
	sa_crs_recovery_equation_group_index = new int*[w];
	sa_crs_recovery_equation_group_index_number = new int[w];
	for (int i = 0; i < w;i++) {
		sa_crs_recovery_equation_group[i] = new int[m*w];
		sa_crs_recovery_equation_group_index_number[i] = 0;
		for (int j = 0; j < m*w; j++) {
			sa_crs_recovery_equation_group[i][j] = 0;
		}
	}
	for (int i = 0; i < m*w;i++) {
		for (int j = 0; j < k*w;j++) {
			if (j/w==failed_disk_id) {
				if (generator_matrix[i*k*w+j]==1) {
					sa_crs_recovery_equation_group[j%w][i] = 1;
					sa_crs_recovery_equation_group_index_number[j%w]++;
				}
			}
		}
	}
	//Initialization: sa_crs_recovery_equation_group_index
	for (int i = 0; i < w; ++i) {
		sa_crs_recovery_equation_group_index[i] = new int[sa_crs_recovery_equation_group_index_number[i]];
		int index = 0;
		for (int j = 0; j < m*w; ++j) {
			if (sa_crs_recovery_equation_group[i][j] == 1) {
				sa_crs_recovery_equation_group_index[i][index++] = j;
			}
		}
	}
	//cout << "Output initialization results: " << endl;
	//for (int i = 0; i < w;i++) {
	//	printf("i=%d: ",i);
	//	for (int j = 0; j < m*w;j++) {
	//		printf("%d",sa_crs_recovery_equation_group[i][j]);
	//	}
	//	printf("\n");
	//}
}

int calculate_row_profit(int k, int w, int row, int faild_disk_id,int *generator_matrix) {
	int row_profit = 0;
	for (int i = 0; i < k*w;i++) {
		if (i/w!= faild_disk_id && generator_matrix[row*k*w+i]==1) {
			row_profit++;
		}
	}
	return row_profit;
}

int calculate_all_profit(int k,int m,int w,int faild_disk_id,int *generator_matrix,int *sa_crs_hybrid_parity_group_selection) {
	int all_profit = 0;
	for (int i = 0; i < k*w;i++) {
		if (i/w!=faild_disk_id) {
			for (int j = 0; j < m*w; j++) {
				if (sa_crs_hybrid_parity_group_selection[j]==1) {//The row is selected as a recovery equation to participate in reconstruction
					if (generator_matrix[j*k*w+i]==1) {
						all_profit++;
						break;
					}
				}
			}
		}
	}
	return all_profit;
}

bool judge_row_selected(int m,int w, int row) {
	bool selected = false;
	for (int i = 0; i < m*w; i++) {
		if (sa_crs_hybrid_parity_group_selection[row] == 1) {
			selected = true;
		}
	}
	return selected;
}

void init_crs_hybrid_parity_group_selection(int k, int m, int w, int faild_disk_id, int *generator_matrix) {
	//Randomly select the appropriate recovery solution
	sa_crs_hybrid_parity_group_selection = new int[m*w];
	sa_crs_hybrid_parity_group_selection_index = new int[w];
	for (int i = 0; i < m*w; i++) {
		sa_crs_hybrid_parity_group_selection[i] = 0;
		//if (i<w) {
		//	sa_crs_hybrid_parity_group_selection[i] = 1;
		//	sa_crs_hybrid_parity_group_selection_index[i] = i;
		//}
	}
	for (int i = 0; i < w; ++i) {
		srand((unsigned)time(NULL));
		int symbol_id = rand() % sa_crs_recovery_equation_group_index_number[i];
		while (sa_crs_hybrid_parity_group_selection[sa_crs_recovery_equation_group_index[i][symbol_id]] == 1) {
			symbol_id = rand() % sa_crs_recovery_equation_group_index_number[i];
		}
		sa_crs_hybrid_parity_group_selection[sa_crs_recovery_equation_group_index[i][symbol_id]] = 1;
		sa_crs_hybrid_parity_group_selection_index[i] = sa_crs_recovery_equation_group_index[i][symbol_id];
	}
	//cout << "Output filtered results:" << endl;
	//for (int i = 0; i < m*w; i++) {
	//	printf("%d ", sa_crs_hybrid_parity_group_selection[i]);
	//}
	//printf("\n");
	//for (int i = 0; i < w; i++) {
	//	printf("%d ", sa_crs_hybrid_parity_group_selection_index[i]);
	//}
	//printf("\n");
}
void init_crs_hybrid_parity_group_selection_best(int k,int m,int w, int* generator_matrix) {
	sa_crs_hybrid_parity_group_selection_best = new int[m*w];
	sa_crs_hybrid_parity_group_selection_best_index = new int[w];
	for (int i = 0; i < m*w; ++i) {
		sa_crs_hybrid_parity_group_selection_best[i] = sa_crs_hybrid_parity_group_selection[i];
	}
	for (int i = 0; i < w; ++i) {
		sa_crs_hybrid_parity_group_selection_best_index[i] = sa_crs_hybrid_parity_group_selection_index[i];
	}
}
void init_crs_hybrid_parity_group_selection_temporary(int k, int m, int w,  int *generator_matrix) {
	sa_crs_hybrid_parity_group_selection_temporary = new int[m*w];
	sa_crs_hybrid_parity_group_selection_temporary_index = new int[w];
	for (int i = 0; i < m*w; i++) {
		sa_crs_hybrid_parity_group_selection_temporary[i] = sa_crs_hybrid_parity_group_selection[i];
	}
	for (int i = 0; i < w; i++) {
		sa_crs_hybrid_parity_group_selection_temporary_index[i] = sa_crs_hybrid_parity_group_selection_index[i];
	}
	/*cout << "Output temporary group: " << endl;
	for (int i = 0; i < m*w; i++) {
		printf("%d", sa_crs_hybrid_parity_group_selection_temporary[i]);
	}
	printf("\n");
	for (int i = 0; i < w; i++) {
		printf("%d", sa_crs_hybrid_parity_group_selection_temporary_index[i]);
	}
	printf("\n");*/
}

void receive_replacement(int m,int w) {
	for (int i = 0; i < m*w; i++) {
		sa_crs_hybrid_parity_group_selection[i] = sa_crs_hybrid_parity_group_selection_temporary[i];
	}
	for (int i = 0; i < w; i++) {
		sa_crs_hybrid_parity_group_selection_index[i] = sa_crs_hybrid_parity_group_selection_temporary_index[i];
	}
}

void remember_best(int m, int w) {
	for (int i = 0; i < m*w; ++i) {
		sa_crs_hybrid_parity_group_selection_best[i] = sa_crs_hybrid_parity_group_selection_temporary[i];
	}
	for (int i = 0; i < w; ++i) {
		sa_crs_hybrid_parity_group_selection_best_index[i] = sa_crs_hybrid_parity_group_selection_temporary_index[i];
	}
}

void print_crs_temporary_solution(int m, int w) {
	/*for (int d = 0; d < w; d++) {
		printf("%d ", sa_crs_hybrid_parity_group_selection_temporary_index[d]);
	}
	cout << "  ";*/
	for (int d = 0; d < m*w;d++) {
		printf("%d", sa_crs_hybrid_parity_group_selection_temporary[d]);
	}
	cout << endl;
}

bool judge_row_selected_temporary(int m, int w, int row) {
	bool selected = false;
	if (sa_crs_hybrid_parity_group_selection_temporary[row] == 1) {
		selected = true;
	}
	return selected;
}

void sa_int2bin(int integer, int *binary, int array_len) {
	int k = 0, n = 0;
	int remain;
	int *temp = new int[array_len];
	do {
		remain = integer % 2;
		integer = integer / 2;
		temp[k++] = remain;
	} while (integer > 0);
	while (k<array_len) {
		temp[k++] = 0;
	}
	while (k >= 0) {
		binary[n++] = temp[--k];
	}
	delete[] temp;
}

bool sa_judge_in_failed_disk_selection(int k,int m,int w,int failed_disk_id,int* generator_matrix,int row, int* temporary_group_id) {
	if (sa_crs_hybrid_parity_group_selection_temporary[row] == 1) {
		return true;
	}
	int temporary_group_id_index = 0;
	for (int s = 0; s < k*w; s++) {
		if (s / w == failed_disk_id) {
			if (generator_matrix[row*k*w + s] == 1) {
				temporary_group_id[temporary_group_id_index] = s%w;
				temporary_group_id_index++;
			}
		}
	}
	int group_id = temporary_group_id[rand() % temporary_group_id_index];
	sa_crs_hybrid_parity_group_selection_temporary[sa_crs_hybrid_parity_group_selection_temporary_index[group_id]] = 0;
	int search_scale = 0;
	int source_num = 0;
	for (int i = 0; i < m*w;i++) {
		if (sa_crs_hybrid_parity_group_selection_temporary[i]==1) {
			source_num++;
		}
	}
	int* temp_array = new int[source_num];
	int index = 0;
	for (int i = 0; i < m*w;i++) {
		if (sa_crs_hybrid_parity_group_selection_temporary[i]==1) {
			temp_array[index++] = i;
		}
	}
	search_scale = 1 << source_num;
	int* binaryArray = new int[source_num];
	int* temp_sum = new int[w];
	for (int i = 0; i < search_scale; i++) {
		memset(temp_sum, 0, sizeof(int)*w);
		sa_int2bin(i, binaryArray, source_num);
		for (int j = 0; j<source_num; j++) {
			if (binaryArray[j] == 1) {
				for (int t = 0; t<w; t++) {
					if (generator_matrix[temp_array[j] * k*w + failed_disk_id*w + t] == temp_sum[t]) {
						temp_sum[t] = 0;
					}
					else {
						temp_sum[t] = 1;
					}
				}
			}
		}
		int t = 0;
		for (t = 0; t<w; t++) {
			if (temp_sum[t] != generator_matrix[row*k*w + failed_disk_id*w + t]) {
				break;
			}
		}
		if (t == w) {
			sa_crs_hybrid_parity_group_selection_temporary[sa_crs_hybrid_parity_group_selection_temporary_index[group_id]] = 1;
			return true;
		}
	}
	delete[] temp_array;
	delete[] temp_sum;
	delete[] binaryArray;
	sa_crs_hybrid_parity_group_selection_temporary[sa_crs_hybrid_parity_group_selection_temporary_index[group_id]] = 1;
	return false;
}

void sa_search_recovery_solution(int k,int m,int w,int failed_disk_id,int *generator_matrix) {
	double K = 0.97, T= k*m*w*w, M = k*m*w*w, L = k*m*w*w;
	double ini = M;
	double remain_times = ini;
	double random_probability = 0;
	int all_profit = 0;
	int all_profit_new = 0;
	int all_profit_best = 0;
	int* temporary_group_id = new int[w];
	int temporary_group_id_index = 0;
	int all_profit_difference = 0;
	while(remain_times > 0 && T > 0.001) {
		srand((unsigned)time(NULL));
		for (int l = 0; l < L; l++) {
			int symbol_id = rand() % (m*w);
			while (sa_judge_in_failed_disk_selection(k, m, w, failed_disk_id, generator_matrix, symbol_id, temporary_group_id)) {
				symbol_id = rand() % (m*w);
			}
			int temporary_group_id_index = 0;
			for (int s = 0; s < k*w; s++) {
				if (s / w == failed_disk_id) {
					if (generator_matrix[symbol_id*k*w + s] == 1) {
						temporary_group_id[temporary_group_id_index] = s%w;
						temporary_group_id_index++;
					}
				}
			}
			int group_id = temporary_group_id[rand() % temporary_group_id_index];
			
			/*cout << "Temporary group before replacement: ";
			print_crs_temporary_solution(m, w);*/
			all_profit = calculate_all_profit(k, m, w, failed_disk_id, generator_matrix,sa_crs_hybrid_parity_group_selection_temporary);
			all_profit_best = calculate_all_profit(k, m, w, failed_disk_id, generator_matrix, sa_crs_hybrid_parity_group_selection_best);
			
			sa_crs_hybrid_parity_group_selection_temporary[sa_crs_hybrid_parity_group_selection_temporary_index[group_id]] = 0;
			sa_crs_hybrid_parity_group_selection_temporary[symbol_id] = 1;
			sa_crs_hybrid_parity_group_selection_temporary_index[group_id] = symbol_id;

			all_profit_new = calculate_all_profit(k, m, w, failed_disk_id, generator_matrix, sa_crs_hybrid_parity_group_selection_temporary);
			/*cout << "  profit:" << all_profit << "	";
			cout << "Temporary group after replacement:";
			print_crs_temporary_solution(m, w);
			cout << "  all_profit_new:" << all_profit_new << "	"; 
			cout << endl;*/
			all_profit_difference = all_profit - all_profit_new;//The difference between the amount of data before replacement and the amount of data after replacement
			if (all_profit_difference > 0) {
				receive_replacement(m, w);
				//cout << "all_profit_best: "<< all_profit_best<<"  all_profit_new: "<< all_profit_new <<"  T:"<< T <<"  remain_times: "<< remain_times  << endl;
				if (all_profit_best > all_profit_new) {
					remember_best(m,w);
				}
			}
			else {
				random_probability = (double)(rand() / (double)RAND_MAX);
				if (exp(all_profit_difference / T)>random_probability) {
					receive_replacement(m, w);
					T = K*T;
				}
				else {
					remain_times--;
					sa_crs_hybrid_parity_group_selection_temporary[symbol_id] = 0;
					sa_crs_hybrid_parity_group_selection_temporary[sa_crs_hybrid_parity_group_selection_index[group_id]] = 1;
					sa_crs_hybrid_parity_group_selection_temporary_index[group_id] = sa_crs_hybrid_parity_group_selection_index[group_id];
				}
			}
		}
	}
	delete[] temporary_group_id;
	delete[] sa_crs_hybrid_parity_group_selection_temporary;
	delete[] sa_crs_hybrid_parity_group_selection_temporary_index;
	delete[] sa_crs_hybrid_parity_group_selection;
	delete[] sa_crs_hybrid_parity_group_selection_index;
	delete[] sa_crs_hybrid_parity_group_selection_best_index;
}

bool judge_include(int* index,int row,int w) {
	bool judge = false;
	for (int i = 0; i < w;i++) {
		if (index[i]==row) {
			judge = true;
		}
	}
	return judge;
}

void backtracking_scheduling(int* index,int* temporary_col,int row,int w,int** recovery_matrix) {
	bool replace = true;
	for (int i = temporary_col[row]; i < w; i++) {
		if (recovery_matrix[row][i] == 1) {
			if (!judge_include(index, i, w)) {
				index[row] = i;
				temporary_col[row] = i;
				replace = false;
				if (row<(w - 1)) {
					backtracking_scheduling(index, temporary_col,row + 1, w, recovery_matrix);
				}
			}
		}
	}
	if (replace) {
		index[row - 1] = -1;
		temporary_col[row - 1]++;
		index[row] = -1;
		temporary_col[row] = 0;
		backtracking_scheduling(index, temporary_col,row - 1, w, recovery_matrix);
	}
}

int* calculate_recovery_index(int k,int m,int w, int failed_disk_id,int* generator_matrix,int* sa_crs_hybrid_parity_group_selection) {
	int** recovery_matrix = new int*[w];
	int row = 0;
	for (int i = 0; i < m*w;i++) {
		if (sa_crs_hybrid_parity_group_selection[i]==1) {
			recovery_matrix[row] = new int[w];
			for (int j = 0; j < k*w;j++) {
				if (j/w==failed_disk_id) {
					recovery_matrix[row][j%w] = generator_matrix[i*k*w + j];
				}
			}
			row++;
		}
	}
	int* recovery_index = new int[w];//the final order of subscript
	int* temporary_col = new int[w];//The subscript order of each line
	for (int i = 0; i < w;i++) {
		recovery_index[i] = -1;
		temporary_col[i] = 0;
	}
	backtracking_scheduling(recovery_index, temporary_col,0,w, recovery_matrix);
	delete[] temporary_col;
	for(int i = 0; i < w; ++i){
		delete[] recovery_matrix[i];
	}
	delete recovery_matrix;
	return recovery_index;
}

int* sa_crs_hybrid_recovery_solution(int k, int m, int w, int failed_disk_id, int *generator_matrix) {
	//cout << "Start initialization: " << endl;
	init_crs_recovery_equation_group(k,m,w,failed_disk_id,generator_matrix);
	init_crs_hybrid_parity_group_selection(k, m, w, failed_disk_id,generator_matrix);
	init_crs_hybrid_parity_group_selection_best(k,m,w,generator_matrix);
	init_crs_hybrid_parity_group_selection_temporary(k, m, w,  generator_matrix);
	sa_search_recovery_solution(k,m,w,failed_disk_id,generator_matrix);
	return sa_crs_hybrid_parity_group_selection_best;
}

int sa_calculate_block_saving(int k, int m, int w, int failed_disk_id, int *generator_matrix, int* sa_crs_hybrid_parity_group_selection) {
	int blocks_savings = 0;
	for (int i = 0; i<k*w; i++) {
		if ((i / w) != failed_disk_id) {
			int tmp_saving = 1;
			for (int j = 0; j<m*w; j++) {
				if (sa_crs_hybrid_parity_group_selection[j] == 1) {
					//This check block is selected to participate in the reconstruction
					if (generator_matrix[j*k*w + i] == 1) {
						tmp_saving = 0;
						break;
					}
				}
			}
			blocks_savings = blocks_savings + tmp_saving;
		}
	}
	return blocks_savings;
}

void sa_crs_print_parity_group_selection(int m, int w, int* sa_crs_hybrid_parity_group_selection) {
	for (int i = 0; i < m*w; i++) {
		printf("%d", sa_crs_hybrid_parity_group_selection[i]);
	}
	printf("\n");
}

void sa_print_crs_solution(int k, int m, int w, map<int, vector<int> > crs_solution) {
	for (int i = 0; i < (k + m); i++) {
		map<int, vector<int> >::iterator crs_solution_iterator = crs_solution.find(i);
		cout << "Node:" << i << "  ";
		if (crs_solution_iterator != crs_solution.end()) {
			for (int j = 0; j < crs_solution_iterator->second.size(); j++) {
				cout << crs_solution_iterator->second[j] << " ";
			}
			cout << endl;
		}
		else {
			cout << endl;
		}
	}
	cout << endl;
}

void sa_get_recovery_solution(int k, int m, int w, int failed_disk_id, int* generator_matrix, int* sa_crs_parity_group_selection, map<int, vector<int> >& crs_solution)
{
	int* tmp_recovery_solution;
	tmp_recovery_solution = new int[k*w];
	int i, j;
	for (i = 0; i < k*w; i++) {
		for (j = 0; j < m*w; j++) {
			if (sa_crs_parity_group_selection[j] == 1) {
				if (generator_matrix[j*k*w + i] == 1) {
					tmp_recovery_solution[i] = 1;
					break;
				}
			}
		}
		if (j == m*w) {
			tmp_recovery_solution[i] = 0;
		}
	}
	for (i = 0; i < k*w; i++) {
		cout << tmp_recovery_solution[i] << " ";
	}
	cout << endl;
	for (i = 0; i < k*w; i++) {
		if (i / w == failed_disk_id) {
			//vector<int> solution_index;
			//crs_solution.insert(map<int, vector<int> >::value_type(i / w, solution_index));
		}
		else if (tmp_recovery_solution[i] == 1) {
			map<int, vector<int> >::iterator crs_solution_iterator = crs_solution.find(i / w);
			if (crs_solution_iterator == crs_solution.end()) {
				vector<int> solution_index;
				solution_index.insert(solution_index.end(), i%w);
				crs_solution.insert(map<int, vector<int> >::value_type(i / w, solution_index));
			}
			else {
				//crs_solution_iterator->second.insert(crs_solution_iterator->second.end(), i%w);
				crs_solution_iterator->second.push_back(i%w);
			}
		}
	}
	for (i = 0; i < m*w; i++) {
		if (sa_crs_parity_group_selection[i] == 1) {
			map<int, vector<int> >::iterator crs_solution_iterator = crs_solution.find(i / w + k);
			if (crs_solution_iterator == crs_solution.end()) {
				vector<int> solution_index;
				solution_index.insert(solution_index.end(), i%w);
				crs_solution.insert(map<int, vector<int> >::value_type(i / w + k, solution_index));
			}
			else {
				crs_solution_iterator->second.insert(crs_solution_iterator->second.end(), i%w);
			}
		}
	}
	delete[] tmp_recovery_solution;
}
