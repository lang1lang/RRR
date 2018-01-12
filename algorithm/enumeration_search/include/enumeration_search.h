#include <limits.h>
int* enumeration_crs_hybrid_recovery_solution(int k, int m, int w, int failed_disk_id, int *generator_matrix);
#define BIG_NUMBER INT_MAX

typedef long long Pathmatrix;    /* 用于存储最短路径下标的数组 */
typedef int ShortPathTable;/* 用于存储到各点最短路径的权值和 */

typedef int* VertexType; /* 顶点类型为int*，表示一个01序列 */
typedef int EdgeType;    /* 边上的权值类型为int */

typedef struct EdgeNode  /* 边表结点 */
{
	long long adjvex;          /* 邻接点域, 存储该顶点对应的下标 */
	EdgeType weight;     /* 用于存储权值,对于非网图可以不需要 */
    struct EdgeNode *next;      /* 链域,指向下一个邻接点 */
} EdgeNode;

typedef struct VextexNode/* 顶点表结点 */
{
	VertexType data;     /* 顶点域,存储顶点信息 */
	EdgeNode *firstedge; /* 边表头指针 */
} VextexNode, AdjList;

typedef struct
{
	AdjList* adjList;
	long long numNodes, numEdges; /* 图中当前顶点数和边数 */
} GraphAdjList;

typedef struct 
{
	long long* node_set;//本层的节点集合
	long long node_num;//本层的节点数
}NodeSet;
