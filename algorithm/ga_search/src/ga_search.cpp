#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<math.h>
#include"ga_search.h"

double cross_prob = 0.8;
double mute_prob = 0.5;
int noOfChromosomes;
int**   ga_crs_recovery_equation_population;		//population
int*    ga_crs_recovery_equation_population_profit;     //the profit for population
double* ga_crs_recovery_equation_population_rate;       //the rate in population
int*    ga_crs_hybrid_parity_group_selection;		//crossover probability

int**   ga_crs_recovery_equation_group;			//group for failed symbols
int**   ga_crs_recovery_equation_group_index;		//remember the parity symbols for recovery a data parity
int***  ga_crs_recovery_equation_group_binary;		//binary numbers, remember the parity symbols for recovery a data parity
int*    ga_crs_recovery_equation_group_index_number;	//symbol numbers for each group
/*
* Converting integer into binary numbers
*/
void ga_int2bin(int integer, int *binary, int array_len) {
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
/*
* Converting binary numbers into integer
*/
void ga_bin2int(int *binary, int &integer, int array_len){
	int j = array_len - 1;
	integer = 0;
	for(int i = 0; i < array_len; ++i){
		integer += binary[i]*(int)pow(2,j--);
	}
}
/*
* Copy int array
*/
void copy_int_array(int len, int* source, int* distance){
	for(int i = 0; i < len; ++i){
		distance[i] =  source[i];
	}
}
/*
* Init ga_crs_recovery_equation_group
*/
void init_crs_recovery_equation_group(int k, int m, int w, int failed_disk_id, int* generator_matrix){
	int search_scale = 0;
	int source_num = 0;

	source_num = w;
	search_scale = 1 << source_num;
	int* binaryArray = new int[source_num];

	ga_crs_recovery_equation_group = new int*[w];
	ga_crs_recovery_equation_group_binary = new int**[w];
	ga_crs_recovery_equation_group_index  = new int*[w];
	ga_crs_recovery_equation_group_index_number = new int[w];//The number for each group
	for(int i = 0; i < w; ++i){
		ga_crs_recovery_equation_group[i] = new int[m*w];
		ga_crs_recovery_equation_group_index_number[i] = 0;
		for(int j = 0; j < m*w; ++j){
			ga_crs_recovery_equation_group[i][j]=0;
		}
	}
	for(int i = 0; i < m*w; ++i){
		for(int j=0; j < k*w; ++j){
			if(j / w == failed_disk_id){
				if(generator_matrix[i*k*w+j] == 1){
					ga_crs_recovery_equation_group[j%w][i]=1;
					ga_crs_recovery_equation_group_index_number[j%w]++;
				}
			}
		}
	}
	for(int i = 0; i < w; ++i){
		ga_crs_recovery_equation_group_binary[i] = new int*[ga_crs_recovery_equation_group_index_number[i]];
		ga_crs_recovery_equation_group_index[i]  = new int[ga_crs_recovery_equation_group_index_number[i]];
		int index = 0;
		for(int j = 0; j < m*w; ++j){
			if(ga_crs_recovery_equation_group[i][j] == 1){
				ga_int2bin(j, binaryArray, source_num);
				ga_crs_recovery_equation_group_binary[i][index] = new int[w];
				ga_crs_recovery_equation_group_index[i][index] = j;
				copy_int_array(w, binaryArray, ga_crs_recovery_equation_group_binary[i][index]);
				index++;
			}
		}
	}
	for(int i = 0; i < w; ++i){
		delete[] ga_crs_recovery_equation_group[i];
	}
	delete   ga_crs_recovery_equation_group;
	delete[] binaryArray;
}
/*
* Judge whether this index can be selected
*/
bool judge_index_in_population(int population_row, int now_group, int gene_number, int w, int len){
	//population_row: now , how many chromosomes in population
	//now_group: now, consider this group
	//gene_number: now, the number of gene in this chromosome
	//len: the length of chromosome
	bool in = false;
	int* binary = new int[w];
	int* index = new int[now_group];
	for(int i = 0; i < now_group * w; i += w){
		for(int j = i; j < i + w; ++j){
			binary[j-i] = ga_crs_recovery_equation_population[population_row][j];
		}
		ga_bin2int(binary, index[i/w], w);
	}
	printf("Now, in this population:\n");
	/*for(int i = 0; i < now_group * w; ++i){
		printf("%d", ga_crs_recovery_equation_population[population_row][i]);
	}
	printf("\n");*/
	for(int i = 0; i < now_group; ++i){
		printf("%d ", index[i]);
	}
	printf("\n");
	int row = 0;//this gene reprensents a parity symbol
	for(int i = 0; i < w; ++i){
		binary[i] = ga_crs_recovery_equation_group_binary[now_group][gene_number][i];
	}
	ga_bin2int(binary, row, w);
	for(int i = 0; i < now_group * w; ++i){
		if(row == index[i]){
			in = true;
		}
	}
	delete[] binary;
	delete[] index;
	return in;
}

/*
* Initiallising population
*/
void init_population(int m, int w){
	noOfChromosomes = m*w;
	srand((unsigned)time(NULL));
	ga_crs_recovery_equation_population = new int*[noOfChromosomes];
	int len = w*w;
	for(int i = 0; i < noOfChromosomes; ++i){
		ga_crs_recovery_equation_population[i] =  new int[len];
		int num = 0;
		for(int j = 0; j < w; ++j){
			int index = rand() % ga_crs_recovery_equation_group_index_number[j];
			printf("index: %d\n", index);
			while(j > 0 && judge_index_in_population(i, j, index, w, len)){
				index = rand() % ga_crs_recovery_equation_group_index_number[j];
				printf("index: %d\n", index);
			}
			for(int k = 0; k < w; ++k){
				ga_crs_recovery_equation_population[i][num++] = ga_crs_recovery_equation_group_binary[j][index][k];
			}
		}
		break;
	}
}
/*
* Calculate the profit for one chromosome
*/
int calculate_chromosome_profit(int* chromosome, int len, int k, int m, int w, int failed_disk_id, int* generator_matrix){
	int* binary = new int[w];
	int* index  = new int[w];
	int profit = 0;
	for(int i = 0; i < len; i += w){
		for(int j = i; j < i + w; ++j){
			binary[j-i] = chromosome[j];
		}
		ga_bin2int(binary, index[i/w], w);
	}
	for(int i = 0; i < k * w; ++i){
		if(i / w != failed_disk_id){
			for(int j = 0; j < w; ++j){
				if(generator_matrix[index[j]*k*w+i] == 1){
					profit++;
					break;
				}		
			}	
		}	
	}
	delete[] binary;
	delete[] index;
	return profit;
}
/*
* Calculate the profit for the population
*/
int* calculate_population_profit(int k, int m, int w, int failed_disk_id, int* generator_matrix){
	ga_crs_recovery_equation_population_profit = new int[noOfChromosomes];
	int len = w*w;
	for(int i = 0; i < noOfChromosomes; ++i){
		ga_crs_recovery_equation_population_profit[i] = calculate_chromosome_profit(ga_crs_recovery_equation_population[i], len, k, m, w, failed_disk_id, generator_matrix);
	}
	return ga_crs_recovery_equation_population_profit;
}
/*
* Rank selection
*/
void rank_chromosomes(int w, int* ga_crs_recovery_equation_population_profit){
	int len = w*w;
	int* temp_population = new int[len];
	int profit = 0;
	for(int i = 0; i < noOfChromosomes -1; ++i){
		for(int j = i + 1; j < noOfChromosomes; ++j){
			if(ga_crs_recovery_equation_population_profit[i] > ga_crs_recovery_equation_population_profit[j]){
				//swap
				profit = ga_crs_recovery_equation_population_profit[i];
				ga_crs_recovery_equation_population_profit[i] = ga_crs_recovery_equation_population_profit[j];
				ga_crs_recovery_equation_population_profit[j] = profit;
				
				copy_int_array(len, ga_crs_recovery_equation_population[i], temp_population);
				copy_int_array(len, ga_crs_recovery_equation_population[j], ga_crs_recovery_equation_population[i]);
				copy_int_array(len, temp_population, ga_crs_recovery_equation_population[j]);
			}
		}
	}
	delete[] temp_population;	
}
/*
* Selection
*/
void rank_selection(int w, int* ga_crs_recovery_equation_poulation_profit){
	int** temp_population;
	temp_population = new int*[noOfChromosomes];
	srand((unsigned)time(NULL));
	rank_chromosomes(w, ga_crs_recovery_equation_poulation_profit);
	//rate sum
	ga_crs_recovery_equation_population_rate = new double[noOfChromosomes];
	int profit_sum = 0;
	int len = w*w;
	for(int i = 0; i < noOfChromosomes; ++i){
		profit_sum += (i+1);
	}
	for(int i = 0; i < noOfChromosomes; ++i){
		ga_crs_recovery_equation_population_rate[i] = (double)(noOfChromosomes -i)/profit_sum;
	}
	for(int i = 0; i < noOfChromosomes; ++i){
		temp_population[i] = new int[len];
		double rate = (double)rand() / RAND_MAX;
		double temp_rate = 0;
		int j = 0;
		for(j = 0; j < noOfChromosomes; ++j){
			temp_rate += ga_crs_recovery_equation_population_rate[j];
			if(temp_rate > rate){
				break;
			}
		}
		if(j == noOfChromosomes){
			j--;
		}
		copy_int_array(len, ga_crs_recovery_equation_population[j], temp_population[i]);
	}
	for(int i = 0; i < noOfChromosomes; ++i){
		copy_int_array(len, temp_population[i], ga_crs_recovery_equation_population[i]);
	}	
}
/*
* Roulette wheel selection
*/
void roulette_wheel_selection_chromosomes(int k, int m, int w, int failed_disk_id, int* generator_matrix, int* ga_crs_recovery_equation_population_profit){
	
}
/*
* The interface for Genetic Algorithm
*/
int* ga_crs_hybrid_recovery_solution(int k, int m, int w, int failed_disk_id, int* generator_matrix){
	init_crs_recovery_equation_group(k, m, w, failed_disk_id, generator_matrix);
	for(int i = 0; i < w; ++i){
		for(int j = 0; j < ga_crs_recovery_equation_group_index_number[i]; ++j){
			for(int k = 0; k < w; ++k){
				printf("%d",ga_crs_recovery_equation_group_binary[i][j][k]);
			}	
			printf(" ");
		}
		printf("\n");
	}
	//Init population
	printf("Output the population:\n");
	init_population(m,w);
	for(int i = 0; i < noOfChromosomes; ++i){
		for(int j = 0; j < w*w; ++j){
			printf("%d", ga_crs_recovery_equation_population[i][j]);
		}
		printf("\n");
	}
	
	//dertimining fitness of chromosome
	int* ga_crs_recovery_equation_population_profit = calculate_population_profit(k, m, w, failed_disk_id, generator_matrix);
	printf("dertimining fitness of chromosome:\n");
	for(int i = 0; i < noOfChromosomes; ++i){
		printf("%d\n", ga_crs_recovery_equation_population_profit[i]);
	}
	//rank selection
	rank_selection(w, ga_crs_recovery_equation_population_profit);
	printf("\n\nafter rank selection:\n");	
	for(int i = 0; i < noOfChromosomes; ++i){
		for(int j = 0; j < w*w; ++j){
			printf("%d", ga_crs_recovery_equation_population[i][j]);
		}
		printf("\n");
	}
	return NULL;
}
