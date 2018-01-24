#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<iostream>
#include<stack>
#include"enumeration_search.h"
using namespace std;
int*  F;//the set of failed symbols
int*** E;//the set of decode equations for reference
int** decoding_equations;//the combination of all decoding equations, E is from the decoding_equations by enumeration search
int* final_decoding_matrix;//the final decoding sub-matrix
long long* decoding_equations_num;//the number of the equations in each recovery equation group E

long long numNodes;//the number of nodes
long long numEdges;//the number of edges

void init_F(int k,int m,int w,int failed_disk_id) {
	F = new int[w];
	for (int i = 0; i < w; ++i) {
		F[i] = failed_disk_id*w + i;
	}
}

void init_decoding_equations(int k,int m,int w) {
	decoding_equations = new int*[(long long)(pow(2,m*w))];//the total number of decoding equations:2^(m*w)
}

void init_E(int k,int m,int w) {
	E = new int**[w];//There are totally a total of w decoding equations for reference, each containing a number of equations.A recovery solution can be obtained by selecting an equation from each group.
}

void init_final_decoding_matrix(int k,int m,int w) {
	final_decoding_matrix = new int[w*(k*w+m*w)];
}

void enumeration_int2bin(long long integer, int *binary, int array_len) {
	int k = 0, n = 0;
	long long remain;
	long long *temp = new long long[array_len];
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

void enumeration_decoding_equations_for_E(int k, int m, int w) {
	decoding_equations_num = new long long[w];//the number of the equations in each recovery equation group E
	int* index = new int[w];//the subscript of the equation in each recovery equation group E
	for (int i = 0; i < w;i++) {
		decoding_equations_num[i] = 0;
		index[i] = 0;
	}
	for (long long i = 0; i < pow(2, m*w); ++i) {
		int can_recovery = -1;//determine which failed symbol can be recovered by the decoding equation
		int failed_number = 0;//number of failure symbols
		for (int j = 0; j < w; ++j) {
			if (decoding_equations[i][F[j]] == 1) {
				failed_number++;
				can_recovery = j;
			}
		}
		if (failed_number == 1) {
			decoding_equations_num[can_recovery]++;
		}
	}
	for (int i = 0; i < w; ++i) {
		E[i] = new int*[decoding_equations_num[i]];
	}

	for (long long i = 0; i < pow(2, m*w);++i) {
		int can_recovery = -1;//determine which failed symbol can be recovered by the decoding equation
		int failed_number = 0;//number of failure symbols
		for (int j = 0; j < w; ++j) {
			if (decoding_equations[i][F[j]] == 1) {
				failed_number++;
				can_recovery = j;
			}
		}
		if (failed_number == 1) {
			E[can_recovery][index[can_recovery]] = new int[k*w+m*w];
			for (int j = 0; j < k*w + m*w;++j) {
				E[can_recovery][index[can_recovery]][j] = decoding_equations[i][j];
			}
			index[can_recovery]++;
		}
	}
	//for (int i = 0; i < w; ++i) {
	//	cout << "E" << i << ":" << endl;
	//	for (int j = 0; j < decoding_equations_num[i]; ++j) {
	//		for (int r = 0; r < k*w + m*w; ++r) {
	//			cout << E[i][j][r];
	//		}
	//		cout << endl;
	//	}
	//}
	delete[] index;
}
void xor_row(int k, int m, int w, long long index, int num, int** tmp_decoding_equations, int* tmp_parity) {
	//num indicates the number of rows selected this time
	//index represents the subscript under the 2^(m*w) scale
	decoding_equations[index] = new int[k*w + m*w];
	for (int i = 0; i < k*w; ++i) {
		int tmp_num = 0;
		for (int row = 0; row < num; ++row) {
			if (tmp_decoding_equations[row][i] == 1) {
				tmp_num++;
			}
		}
		if (tmp_num % 2 == 0) {
			decoding_equations[index][i] = 0;
		}
		else {
			decoding_equations[index][i] = 1;
		}
	}
	for (int i = k*w; i < k*w + m*w; ++i) {
		decoding_equations[index][i] = tmp_parity[i - k*w];
	}
}

void enumeration_decoding_equations(int k, int m, int w, int failed_disk_id, int* generator_matrix) {
	//Constructing an exponential level of solution space
	init_decoding_equations(k, m, w);
	init_F(k,m,w,failed_disk_id);
	init_E(k,m,w);
	long long search_scale = 1 << (m*w);
	int* binaryArray = new int[m*w];
	for (long long i = 0; i < search_scale; ++i) {
		enumeration_int2bin(i, binaryArray, m*w);
		int num = 0;
		for (int j = 0; j < m*w; ++j) {
			if (binaryArray[j] == 1) {
				++num;
			}
		}
		int** tmp_decoding_equations = new int*[num];
		int* tmp_parity = new int[m*w];
		for (int j = 0; j < m*w;++j) {
			tmp_parity[j] = 0;
		}
		int index = 0;
		for (int j = 0; j < m*w;++j) {
			if (binaryArray[j] == 1) {
				tmp_decoding_equations[index] = new int[k*w];
				for (int copy = 0; copy < k*w;++copy) {
					tmp_decoding_equations[index][copy] = generator_matrix[j*k*w + copy];
				}
				tmp_parity[j] = 1;
				index++;
			}
		}
		//all decoding equations under this combination are written to tmp_decoding_equations
		xor_row(k, m, w, i,num, tmp_decoding_equations, tmp_parity);
		for(int j = 0;j < num; ++j){
			delete[] tmp_decoding_equations[j];
		}
		delete tmp_decoding_equations;
		delete[] tmp_parity;
	}
	//cout << "\n\n\n" << endl;
	//for (int i = 0; i < pow(2, m*w);++i) {
	//	cout << "row: "<<i<<" " ;
	//	for (int j = 0; j < k*w + m*w;++j) {
	//		cout << decoding_equations[i][j];
	//	}
	//	cout << endl;
	//}
	//cout << endl << endl;
	enumeration_decoding_equations_for_E(k, m, w);
	for(long long i = 0; i < search_scale; ++i){
		delete[] decoding_equations[i];
	}
	delete decoding_equations;
	delete[] F;
	delete[] binaryArray;
}

void calculate_node_edge_numbers(int k, int m, int w) {
	long long SUM = 1;
	long long num = 1;
	for (int i = 0; i < w; ++i) {
		num = num * decoding_equations_num[i];
		//cout << "i= " << i << " "<< decoding_equations_num[i] << endl;
		SUM = SUM + num;
	}

	numNodes = SUM;
	numEdges = SUM - 1;
}
int calculate_ones(int k,int m,int w, int* equations) {
	int sum = 0;
	for (int i = 0; i < k*w + m*w;++i) {
		if (equations[i] == 1) {
			sum++;
		}
	}
	return sum;
}
/**
 * creating a layer in a critical chain chart
*/
NodeSet* CreateOneLayer(GraphAdjList *Gp,NodeSet* nodeset,int symbol_id,long long *index,int k,int m,int w) {
	//node_set represents the set of nodes on the upper layer
	//node_num represents the number of nodes on the upper layer
	//symbol_id represents the index of the failed symbol in all
	//index represents the node index on the graph
	NodeSet* nodeset_next = new NodeSet;
	long long num = 1;
	for (int i = 0; i <= symbol_id; ++i) {
		num = num * decoding_equations_num[i];
	}
	nodeset_next->node_num = num;
	nodeset_next->node_set = new long long[nodeset_next->node_num];
	long long in = 0;
	for (long long i = 0; i < nodeset->node_num; ++i) {
		for (long long j = 0; j < decoding_equations_num[symbol_id]; ++j) {
			Gp->adjList[*index].data = new int[k*w + m*w];
			for (int r = 0; r < k*w + m*w;++r) {
				Gp->adjList[*index].data[r] = Gp->adjList[nodeset->node_set[i]].data[r] | E[symbol_id][j][r];
			}
			Gp->adjList[*index].firstedge = NULL;
			//set up a set of edges
			EdgeNode *pe = new EdgeNode;
			pe->adjvex = *index;
			pe->weight = calculate_ones(k,m,w,Gp->adjList[*index].data) - calculate_ones(k,m,w,Gp->adjList[nodeset->node_set[i]].data);
			pe->next = Gp->adjList[nodeset->node_set[i]].firstedge;
			Gp->adjList[nodeset->node_set[i]].firstedge = pe;
			nodeset_next->node_set[in] = *index;
			for (int r = 0; r < k*w + m*w; ++r) {
				Gp->adjList[*index].data[r] = E[symbol_id][j][r];
			}
			++(*index);
			++in;
		}
	}
	delete nodeset;
	return nodeset_next;
}
/*
* Creating a critical chain chart
*/
void CreateALGraph(int k,int m,int w,GraphAdjList *Gp) {
	Gp->numNodes = numNodes;//the number of nodes in the graph
	Gp->numEdges = numEdges;//the number of edges in the graph
	Gp->adjList = new AdjList[numNodes];//allocate memory space for each node
	//the following is the initialization of node Z
	long long node_index = 0;
	Gp->adjList[node_index].data = new int[k*w+m*w];
	for (int i = 0; i < k*w + m*w; ++i) {
		Gp->adjList[node_index].data[i] = 0;
	}
	Gp->adjList[node_index].firstedge = NULL;
	node_index++;
	//the following is the set of nodes on the upper layer of the graph
	NodeSet* nodeset = new NodeSet;
	nodeset->node_num = 1;
	nodeset->node_set = new long long[nodeset->node_num];
	nodeset->node_set[0] = node_index - 1;
	for (int i = 0; i < w; ++i) {
		nodeset=CreateOneLayer(Gp, nodeset, i, &node_index, k, m, w);
	}
	delete nodeset;
	for(int i=0; i<w; ++i){
		for(int j=0; j<decoding_equations_num[i]; ++j){
			delete[] E[i][j];
		}
		delete[] E[i];
	}
	/*EdgeNode* edgenode;
	cout << "Nodes in the graph:" << endl;
	for (int i = 0; i < Gp->numNodes;++i) {
		cout << "Node" << i << " = ";
		for (int j = 0; j < k*w + m*w;++j) {
			 cout<<Gp->adjList[i].data[j];
		}
		cout << endl;
	}
	cout << "Edges in the graph:" << endl;
	for (int i = 0; i < Gp->numNodes;++i) {
		cout << "Edge" << i << "= ";
		edgenode = Gp->adjList[i].firstedge;
		while (edgenode != NULL) {
			cout << "->" << edgenode->adjvex << ":" << edgenode->weight << "; ";
			edgenode = edgenode->next;
		}
		cout << endl;
	}*/
}

void ShortestPath_Dijkstra(GraphAdjList Gp,int k,int m,int w,long long v0,Pathmatrix *P,ShortPathTable *D) {
	EdgeNode* edgenode;
	long long v = 0, s = 0, n = 0;
	int min = 0;
	long long *final;
	final = new long long[Gp.numNodes];
	for (v = 0; v < Gp.numNodes; ++v) {
		final[v] = 0;
		D[v] = BIG_NUMBER;
		edgenode = Gp.adjList[v0].firstedge;
		while (edgenode != NULL) {
			if (edgenode->adjvex == v) {
				D[v] = edgenode->weight;
			}
			edgenode = edgenode->next;
		}
		P[v] = 0;
	}
	D[v0] = 0;
	final[v0] = 1;
	//Start the main loop and get the shortest distance from v0 to a certain V node each time
	for (v = 1; v < Gp.numNodes; ++v) {
		min = BIG_NUMBER;
		for (s = 0; s < Gp.numNodes; ++s) {
			if (!final[s] && D[s] < min) {
				n = s;
				min = D[s];
			}
		}
		final[n] = 1;
		for (s = 0; s < Gp.numNodes; ++s) {
			int weight = BIG_NUMBER;
			edgenode = Gp.adjList[n].firstedge;
			while (edgenode != NULL) {
				if (edgenode->adjvex == s) {
					weight = edgenode->weight;
				}
				edgenode = edgenode->next;
			}
			if (weight < BIG_NUMBER && !final[s] && (min+weight < D[s])) {
				D[s] = min + weight;
				P[s] = n;
			}
		}
	}
	delete[] final;
}

void find_rows(GraphAdjList Gp,int k,int m,int w, Pathmatrix *P, ShortPathTable *D) {
	long long SUM = 1;
	long long num = 1;
	long long node_id = 0;
	int min = BIG_NUMBER;
	int index = 0;
	for (int i = 0; i < w - 1; ++i) {
		num = num * decoding_equations_num[i];
		SUM = SUM + num;
	}
	for (long long i = SUM; i < Gp.numNodes; ++i) {
		if (D[i] < min) {
			min = D[i];
			node_id = i;
		}
	}
	stack<long long> st;
	st.push(P[node_id]);
	long long n = st.top();
	while (n != 0) {
		st.push(P[n]);
		n = st.top();
	}
	while (!st.empty()) {
		n = st.top();
		st.pop();
		if (n != 0) {
			for (int i = 0; i < k*w + m*w; ++i) {
				final_decoding_matrix[index++] = Gp.adjList[n].data[i];
			}
		}
	}
	for (int i = 0; i < k*w + m*w; ++i) {
		final_decoding_matrix[index++] = Gp.adjList[node_id].data[i];
	}
	delete[] decoding_equations_num;
}

void create_decoding_equations(GraphAdjList Gp,int k,int m,int w, Pathmatrix *P, ShortPathTable *D, int *generator_matrix) {
	init_final_decoding_matrix(k, m, w);
	find_rows(Gp, k, m, w, P, D);
	//Output decoding matrix
	/*cout << "Output decoding matrix: " << endl;
	for (int i = 0; i < w; ++i) {
		for (int j = 0; j < k*w + m*w; ++j) {
			cout << final_decoding_matrix[i*(k*w + m*w) + j];
		}
		cout << endl;
	}*/
	delete[] P;
	delete[] D;
}

int enumeration_block_saving(int k, int m, int w, int failed_disk_id){
	int blocks_savings = 0;
	for(int i = 0; i < k*w+m*w; ++i){
		if((i / w) != failed_disk_id){
			int tmp_savings = 1;
			for(int j = 0; j < w; ++j){
				if(final_decoding_matrix[j*(k*w+m*w)+i] == 1){
					tmp_savings = 0;
					break;
				}
			}
			blocks_savings += tmp_savings;
		}
	}
	return blocks_savings;
}

void enumeration_crs_hybrid_recovery_solution(int k, int m, int w, int failed_disk_id, int *generator_matrix) {
	enumeration_decoding_equations(k,m,w,failed_disk_id,generator_matrix);
	calculate_node_edge_numbers(k,m,w);//Calculating the number of nodes and edges
	GraphAdjList GL;
	CreateALGraph(k, m, w, &GL);//Creating node sets and edge sets of Graphs
	//Using the Dijkstra algorithm to find the shortest path
	Pathmatrix *P;
	P = new Pathmatrix[GL.numNodes];
	ShortPathTable *D;
	D = new ShortPathTable[GL.numNodes];
	ShortestPath_Dijkstra(GL, k, m, w, 0, P, D);
	//Building recovery equation set
	create_decoding_equations(GL, k, m, w, P, D,generator_matrix);
}
