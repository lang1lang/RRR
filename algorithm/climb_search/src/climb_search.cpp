#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include<iostream>
#include<string>
#include<vector>
#include<map>
#include "climb_search.h"
using namespace std;

int* crs_hybrid_parity_group_selection;
int* crs_final_recovery_parity_vector;
int* rows_intersection_infor;
int crs_hybrid_profit;
int crs_final_hybrid_profit;
double crs_final_replace_recovery_cost;
double crs_replace_recovery_cost;

int count_rows_intersections_num(int m, int k, int w, int failed_disk_id, int param_row1, int param_row2, int *generator_matrix) {
    //row:param_row1 VS row:param_row2
    int intersections_count=0;
    for (int i=0;i<(k*w);i++) {
        if ((i/w)!=failed_disk_id) {//i represents good blocks
            if ((generator_matrix[param_row1*k*w+i]==0)&&(generator_matrix[param_row2*k*w+i]==0)) {
                intersections_count++;
            }
        }
    }
    return intersections_count;

}

int different_failed_blocks(int m, int k, int w, int failed_disk_id, int param_row1, int param_row2, int *generator_matrix) {
    for (int j=0;j<w;j++) {
        if (generator_matrix[param_row1*k*w+failed_disk_id*w+j]!=generator_matrix[param_row2*k*w+failed_disk_id*w+j]) {
            return 1;
        }
    }
    return 0;
}

void construct_rows_intersection_infor_matrix(int m, int k, int w, int failed_disk_id, int *generator_matrix)
{
	rows_intersection_infor= new int[(m*w)*(m*w)];
	for (int i=0;i<(m*w);i++) {
        for (int j=i;j<(m*w);j++) {
            if (i==j) {
                rows_intersection_infor[i*m*w+j]=-1;
            } else {
                //judge whether the i-th row and the j-th row recover the same block , 1 represents difference, block represents w symbols' combination 
                if (different_failed_blocks(m,k,w,failed_disk_id,i,j,generator_matrix)) {
                    rows_intersection_infor[i*m*w+j]=count_rows_intersections_num(m,k,w,failed_disk_id, i,j,generator_matrix);
                } else {
                    rows_intersection_infor[i*m*w+j]=-1;
                }
                rows_intersection_infor[j*m*w+i]=rows_intersection_infor[i*m*w+j];
            }
        }
    }
}

int calculate_blocks_savings( int m, int k, int w, int failed_disk_id, int *generator_matrix, int *crs_parity_group_selection) {
    int blocks_savings=0;
    for (int i=0;i<k*w;i++) {
        if ((i/w)!=failed_disk_id) {
            int tmp_saving=1;
            for (int j=0;j<m*w;j++) {
                if (crs_parity_group_selection[j]==1) {
					//This parity symbol is selected to participate in recovery
                    if (generator_matrix[j*k*w+i]==1) {
                        tmp_saving=0;
                        break;
                    }
                }
            }
            blocks_savings=blocks_savings+tmp_saving;
        }
    }
    return blocks_savings;
}

void int2bin(int integer, int *binary, int array_len) {
    int k = 0, n = 0;
    int remain;
    int *temp = new int[array_len];
    do {
        remain = integer % 2;
        integer = integer / 2;
        temp[k++] = remain;
    } while (integer > 0);
    //Fill 0 into the convert array
    while (k<array_len) {
        temp[k++]=0;
    }
    while (k >= 0) {
        binary[n++] = temp[--k];
    }
    delete[] temp; 
}

//Judging linear correlation
int not_in_failed_disk_selection(int m, int k, int w, int failed_disk_id, int *generator_matrix,int row) {
    int i, j, t;
    j=0;
    if (crs_hybrid_parity_group_selection[row]==1) {
        return 0;
    }//The line row is already in the selection and cannot be replaced again
    int source_num, search_scale;
    search_scale=0;
    source_num=0; 
    for (i=0;i<m*w;i++) {
		//cout << crs_hybrid_parity_group_selection[i];
		if (crs_hybrid_parity_group_selection[i]==1) {
            source_num++;
        }
    }//An option has been deleted, and it should be w-1
	//cout << endl;
    int* temp_array = new int[source_num];
    for (i=0;i<m*w;i++) {
        if (crs_hybrid_parity_group_selection[i]==1) {
            temp_array[j]=i;
            j++;
        }
    }//temp_array saves: the index of 1 in crs_hybrid_parity_group_selection, which represents which parity symbol participates in reconstruction
    search_scale=1<<source_num; //Be equal to 2^source_num,When source_num is equal to 2, search_scale is equal to 4
	//cout << "search_scale=  " << search_scale << "   source_num=  " << source_num <<"  row=  "<< row << endl;
	/*system("pause");*/
	int* binaryArray=new int[source_num];
    int* temp_sum = new int[w];
    for (i=0;i<search_scale;i++) {       //i = 0 1 2 3
        memset(temp_sum,0,sizeof(int)*w);
        int2bin(i,binaryArray,source_num);//0 1 2 3 Converted to array elements
		/*cout << "Output binaryArray" << endl;
		for (int a = 0; a < source_num; a++) {
			cout << binaryArray[a];
		}
		cout << endl;*/
		for (j=0;j<source_num;j++) {
            if (binaryArray[j]==1) {
                for (t=0;t<w;t++) {
                    if (generator_matrix[temp_array[j]*k*w+failed_disk_id*w+t]==temp_sum[t]) {//µÚjÐÐ
                        temp_sum[t]=0;
                    } else {
                        temp_sum[t]=1;
                    }
                }
            }
        }
        for (t=0;t<w;t++) {
            if (temp_sum[t]!=generator_matrix[row*k*w+failed_disk_id*w+t]) {
				//cout << "temp_sum: "<<temp_sum[t]<<"  generator_matrix: "<<generator_matrix[row*k*w + failed_disk_id*w + t]<< endl;
				break;
            }
        }
        if (t==w) {
            return 0;
        }
    }
    delete[] temp_array;
    delete[] temp_sum;
    delete[] binaryArray;
    return 1;
}

int judge_the_better_candidate(int m, int w, int failed_disk_id, int candidate_one, int candidate_two) {
    
    int candidate1_sum, candidate2_sum;
    candidate1_sum=0;
    candidate2_sum=0;

	//printf("candidate_one:%d candidate_two:%d\n", candidate_one, candidate_two);
    for (int i=0;i<(m*w);i++) {
		if ((i != candidate_one) && (i != candidate_two) && ((i / w) != failed_disk_id)) {
			candidate1_sum = candidate1_sum + rows_intersection_infor[candidate_one*m*w + i];
		}
    }
    for (int i=0;i<(m*w);i++) {
		if ((i != candidate_one) && (i != candidate_two) && ((i / w) != failed_disk_id)) {
			candidate2_sum = candidate2_sum + rows_intersection_infor[candidate_two*m*w + i];
		}
    }
    if (candidate1_sum>=candidate2_sum) {
        return 1;
    } else {
        return 0;
    }
}

int crs_better_recovery_exist(int m, int k, int w, int failed_disk_id, int* candidate_replace_parity_groups, int *generator_matrix) {

    int i, j;
    int optimal_j=-1; 
    int selected_candidate=-1;//Selected candidate
    for (i=0;i<(m*w);i++) {
        if (candidate_replace_parity_groups[i]==1) {
            for (j=0;j<(m*w);j++) {
                if (crs_hybrid_parity_group_selection[j]==1) {
                    crs_hybrid_parity_group_selection[j]=0;//Firstly do not select this line
                    if (not_in_failed_disk_selection(m,k,w,failed_disk_id,generator_matrix, i)) {//The judgment statement is to judge the linear correlation
						//When i = 3, j = 0, the return value is 1
                        crs_hybrid_parity_group_selection[i]=1;
						//0 1 1 1 0 0	
                        int savings=calculate_blocks_savings(m,k,w,failed_disk_id, generator_matrix,crs_hybrid_parity_group_selection);
						//printf("j:%d optimal_j:%d savings:%d crs_hybrid_profit:%d\n",j, optimal_j,savings,crs_hybrid_profit);
						if (savings>crs_hybrid_profit) {
                            crs_hybrid_profit=savings;
                            optimal_j=j;
                            selected_candidate=i;
                        } else if (savings==crs_hybrid_profit) {
							if (optimal_j == -1) {
								crs_hybrid_profit = savings;
								optimal_j = j;
								selected_candidate = i;
							}
							else if(judge_the_better_candidate(m, w, failed_disk_id,j,optimal_j)==1) {
                                crs_hybrid_profit=savings;
                                optimal_j=j;
                                selected_candidate=i;
                            }
                        }
                        crs_hybrid_parity_group_selection[i]=0;
                    }
                    crs_hybrid_parity_group_selection[j]=1;
                }
            }
        }
    }
    if (optimal_j!=-1) {
        crs_hybrid_parity_group_selection[optimal_j]=0;
        crs_hybrid_parity_group_selection[selected_candidate]=1;
    }
    return selected_candidate;
}

void crs_adjust_recovery_vector_in_adjustment(int m, int k, int w, int failed_disk_id, int fixed_parity_node, int star_parity_node, int pre_fixed_parity_node, int *generator_matrix) {
	//This function will replace parity node star_parity_disk using parity node fixed_parity_node
	int i, j;
	int tmp_replaced;

	int tmp_crs_hybrid_profit;
	int* tmp_crs_failed_disk_selection = new int[m*w];

	tmp_crs_hybrid_profit = crs_hybrid_profit;
	for (i = 0; i<(m*w); i++) {
		tmp_crs_failed_disk_selection[i] = crs_hybrid_parity_group_selection[i];
	}

	int* candidate_replace_parity_groups = new int[m*w];

	for (i = 0; i<m; i++) {

		if (((i<pre_fixed_parity_node) || (i == star_parity_node)) || (i<fixed_parity_node)) {

			for (j = 0; j<(m*w); j++) {

				if ((j / w) == i) {
					candidate_replace_parity_groups[j] = 1;
				}
				else {
					candidate_replace_parity_groups[j] = 0;
				}

			}

			while ((tmp_replaced = crs_better_recovery_exist(m, k, w, failed_disk_id, candidate_replace_parity_groups, generator_matrix)) != -1) {

				candidate_replace_parity_groups[tmp_replaced] = 0;
			}

		}

	}

	if (crs_hybrid_profit <= tmp_crs_hybrid_profit) {

		crs_hybrid_profit = tmp_crs_hybrid_profit;
		for (i = 0; i<(m*w); i++) {
			crs_hybrid_parity_group_selection[i] = tmp_crs_failed_disk_selection[i];
		}
	}
	else {

		//printf("before adjust is %d and after adjust is %d\n", tmp_crs_hybrid_profit, crs_hybrid_profit);
	}

	delete[] tmp_crs_failed_disk_selection;
}

void crs_adjust_recovery_vector(int m, int k, int w, int failed_disk_id, int fixed_parity_disk, int star_parity_disk, int *generator_matrix) {
	//using fixed_parity_disk to replace star_parity_disk
	int i, j;
	int tmp_replaced;

	int  tmp_crs_hybrid_profit;
	int* tmp_crs_failed_disk_selection = new int[m*w];

	tmp_crs_hybrid_profit = crs_hybrid_profit;
	for (i = 0; i<(m*w); i++) {
		tmp_crs_failed_disk_selection[i] = crs_hybrid_parity_group_selection[i];
	}

	int* candidate_replace_parity_groups = new int[m*w];

	for (i = 0; i<m; i++) {

		if ((i<fixed_parity_disk) || (i == star_parity_disk)) {

			for (j = 0; j<(m*w); j++) {

				if ((j / w) == i) {
					candidate_replace_parity_groups[j] = 1;
				}
				else {
					candidate_replace_parity_groups[j] = 0;
				}
			}

			while ((tmp_replaced = crs_better_recovery_exist(m, k, w, failed_disk_id, candidate_replace_parity_groups, generator_matrix)) != -1) {
				candidate_replace_parity_groups[tmp_replaced] = 0;
			}
			crs_adjust_recovery_vector_in_adjustment(m, k, w, failed_disk_id, i, star_parity_disk, fixed_parity_disk, generator_matrix);
			//using parity node i to replace parity node star_parity_disk
		}

	}

	for (i = 0; i<(m*w); i++) {

		if ((i / w) == fixed_parity_disk) {
			candidate_replace_parity_groups[i] = 1;
		}
		else {
			candidate_replace_parity_groups[i] = 0;
		}
	}

	while ((tmp_replaced = crs_better_recovery_exist(m, k, w, failed_disk_id, candidate_replace_parity_groups, generator_matrix)) != -1) {

		candidate_replace_parity_groups[tmp_replaced] = 0;
	}

	crs_adjust_recovery_vector_in_adjustment(m, k, w, failed_disk_id, fixed_parity_disk, star_parity_disk, fixed_parity_disk, generator_matrix);
	if (crs_hybrid_profit <= tmp_crs_hybrid_profit) {
		crs_hybrid_profit = tmp_crs_hybrid_profit;
		for (i = 0; i<(m*w); i++) {
			crs_hybrid_parity_group_selection[i] = tmp_crs_failed_disk_selection[i];
		}
	}

	delete[] tmp_crs_failed_disk_selection;
	delete[] candidate_replace_parity_groups;

	//printf("Adjustment profit is %d\n", (tmp_crs_hybrid_profit - crs_hybrid_profit));
}

void crs_store_selection_to_final(int m, int w) {
	for (int i = 0; i<(m*w); i++) {
		crs_final_recovery_parity_vector[i] = crs_hybrid_parity_group_selection[i];
	}
}

int* crs_hybrid_recovery_solution(int m, int k, int w, int failed_disk_id,int *generator_matrix)
{
	if ( failed_disk_id>=k ) {
        cout<<"ERROR: Parity Disk Recovery Is Not Considered!\n" <<endl;
        return NULL;
    }
	crs_hybrid_profit=0;//Initialize the cost to 0
	crs_final_hybrid_profit = 0;
	int* candidate_replace_parity_groups = new int[m*w];
	crs_hybrid_parity_group_selection = new int[m*w];
	crs_final_recovery_parity_vector = new int[m*w];

	construct_rows_intersection_infor_matrix(m,k,w,failed_disk_id,generator_matrix);

	for (int i=0;i<(m*w);i++) {
        crs_hybrid_parity_group_selection[i]=0;
        candidate_replace_parity_groups[i]=0;
    }//Initialization is all unselected

	int *optimal_conventional_recovery=new int[m];//The best traditional method
	for (int i=0;i<m;i++) {
        optimal_conventional_recovery[i]=1;
    }//Assign all parity nodes to 1

	for (int j=0;j<m;j++) 
	{
		if (optimal_conventional_recovery[j]==1)
		{
			for (int i=0;i<(m*w);i++) {
                if ((i/w)==j) {//parity data on the j-th parity node
                    crs_hybrid_parity_group_selection[i]=1;
                } else {
                    crs_hybrid_parity_group_selection[i]=0;
                }
            }//Select all the symbol on the first check disk according to the traditional method
			crs_hybrid_profit=calculate_blocks_savings(m,k,w,failed_disk_id,generator_matrix,crs_hybrid_parity_group_selection);
			int s;
			for (int t=0;t<m;t++)
			{
				if (t!=j) 
				{//Consider the next parity node sequentially
					for (int i=0;i<(m*w);i++) {
                        if ((i/w)==t) {//the parity data on the t-th parity node
                            candidate_replace_parity_groups[i]=1;
                        } else {
                            candidate_replace_parity_groups[i]=0;
                        }
                    }
					int tmp_replaced = crs_better_recovery_exist(m, k, w, failed_disk_id, candidate_replace_parity_groups, generator_matrix);
					while (tmp_replaced!=-1) {
                        candidate_replace_parity_groups[tmp_replaced]=0;
						
						tmp_replaced = crs_better_recovery_exist(m, k, w, failed_disk_id, candidate_replace_parity_groups, generator_matrix);
                    }
					crs_adjust_recovery_vector(m, k, w, failed_disk_id, t, j, generator_matrix);
				}
			}
		}
		if (crs_hybrid_profit>=crs_final_hybrid_profit) {
			crs_final_hybrid_profit = crs_hybrid_profit;
			crs_store_selection_to_final(m, w);
		}
	}
	delete[] candidate_replace_parity_groups;
	delete[] crs_hybrid_parity_group_selection;
	delete[] optimal_conventional_recovery;
	delete[] rows_intersection_infor;
	return crs_final_recovery_parity_vector;
}

void crs_print_parity_group_selection(int m, int k, int w, int failed_disk_id, int* crs_parity_group_selection)
{
	for(int i=0;i<m*w;i++)
	{
		cout<<crs_parity_group_selection[i];
	}
	cout<<endl;
}

void get_recovery_solution(int k, int m, int w, int failed_disk_id, int* generator_matrix,int* crs_parity_group_selection, map<int, vector<int> >& crs_solution)
{
	int* tmp_recovery_solution;
	tmp_recovery_solution = new int[k*w];
	int i, j;
	for (i = 0; i < k*w;i++) {
		for (j = 0; j < m*w;j++) {
			if (crs_parity_group_selection[j] == 1) {
				if (generator_matrix[j*k*w + i] == 1) {
					tmp_recovery_solution[i] = 1;
					break;
				}
			}
		}
		if (j==m*w) {
			tmp_recovery_solution[i] = 0;
		}
	}
	for (i = 0; i < k*w;i++) {
		cout << tmp_recovery_solution[i] << " ";
	}
	cout << endl; 
	for (i = 0; i < k*w;i++) {
		if (i/w==failed_disk_id) {
			//vector<int> solution_index;
			//crs_solution.insert(map<int, vector<int> >::value_type(i / w, solution_index));
		}
		else if(tmp_recovery_solution[i]==1){
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
	for (i = 0; i < m*w;i++) {
		if (crs_parity_group_selection[i]==1) {
			map<int, vector<int> >::iterator crs_solution_iterator = crs_solution.find(i/w+k);
			if (crs_solution_iterator == crs_solution.end()) {
				vector<int> solution_index;
				solution_index.insert(solution_index.end(), i%w);
				crs_solution.insert(map<int, vector<int> >::value_type(i/w+k, solution_index));
			}
			else {
				crs_solution_iterator->second.insert(crs_solution_iterator->second.end(), i%w);
			}
		}
	}
	//for (i = 0; i < m*w;i++) {
	//	map<int, vector<int> >::iterator crs_solution_iterator = crs_solution.find(i / w + k);
	//	if (crs_solution_iterator == crs_solution.end()) {
	//		/*vector<int> solution_index;
	//		crs_solution.insert(map<int, vector<int> >::value_type(i / w + k, solution_index));*/
	//	}
	//}
	delete[] tmp_recovery_solution;
}

void print_crs_solution(int k,int m,int w ,map<int, vector<int> > crs_solution) {
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
int calculate_block_saving(int k, int m, int w, int failed_disk_id, int *generator_matrix, int* sa_crs_hybrid_parity_group_selection) {
	int blocks_savings = 0;
	for (int i = 0; i<k*w; i++) {
		if ((i / w) != failed_disk_id) {
			int tmp_saving = 1;
			for (int j = 0; j < m*w; j++) {
				if (sa_crs_hybrid_parity_group_selection[j] == 1) {
					//This parity symbol is selected to participate in reconstruction
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
