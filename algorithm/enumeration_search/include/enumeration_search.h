#include <limits.h>
void enumeration_crs_hybrid_recovery_solution(int k, int m, int w, int failed_disk_id, int *generator_matrix);
int enumeration_block_saving(int k, int m, int w, int failed_disk_id);
#define BIG_NUMBER INT_MAX

typedef long long Pathmatrix;
typedef int ShortPathTable;

typedef int* VertexType; /* The vertex type is int*, representing a 01 sequence */
typedef int EdgeType;    /* The weight type on the edge is int */

typedef struct EdgeNode  /* edge */
{
	long long adjvex;          /* Subscript corresponding to the vertices */
	EdgeType weight;     	   /* The weight on the edge */
    struct EdgeNode *next;         /* Point to the next node */
} EdgeNode;

typedef struct VextexNode/* vertex */
{
	VertexType data;     /* Vertex information */
	EdgeNode *firstedge; /* The head pointer of a vertex table */
} VextexNode, AdjList;

typedef struct
{
	AdjList* adjList;
	long long numNodes, numEdges; /* The number of vertices and the number of edges in the current graph */
} GraphAdjList;

typedef struct 
{
	long long* node_set;   /* The set of nodes in this layer */
	long long node_num;    /* The number of nodes in this layer */
}NodeSet;
