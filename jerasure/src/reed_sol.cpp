#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "reed_sol.h"
#include "galois.h"


#define talloc(type, num) (type *) malloc(sizeof(type)*(num))

int *reed_sol_vandermonde_coding_matrix(int k, int m, int w)
{
	int i, j;
	int *vdm, *dist;

	vdm = reed_sol_big_vandermonde_distribution_matrix(k+m, k, w);
	if (vdm == NULL) return NULL;
	dist = talloc(int, m*k);
	if (dist == NULL) {
		free(vdm);
		return NULL;
	}

	i = k*k;
	for (j = 0; j < m*k; j++) {
		dist[j] = vdm[i];
		i++;
	}
	free(vdm);
	return dist;
}

int *reed_sol_big_vandermonde_distribution_matrix(int rows, int cols, int w)
{
  int *dist;
  int i, j, k;
  int sindex, srindex, siindex, tmp;

  if (cols >= rows) return NULL;
  
  dist = reed_sol_extended_vandermonde_matrix(rows, cols, w);
  if (dist == NULL) return NULL;

          sindex = 0;
		  for (i = 1; i < cols; i++) 
		  {
				sindex += cols;//每一行的第一个元素下标：sindex
				/* Find an appropriate row -- where i,i != 0 */
				srindex = sindex+i;//每一行的第i个元素下标：srindex
				for (j = i; j < rows && dist[srindex] == 0; j++) 
					srindex += cols;
				if (j >= rows) 
				{   /* This should never happen if rows/w are correct */
				  fprintf(stderr, "reed_sol_big_vandermonde_distribution_matrix(%d,%d,%d) - couldn't make matrix\n", 
						 rows, cols, w);
				  assert(0);
				}		 
				/* If necessary, swap rows */
				if (j != i) {
				  srindex -= i;
				  for (k = 0; k < cols; k++) {
					tmp = dist[srindex+k];
					dist[srindex+k] = dist[sindex+k];
					dist[sindex+k] = tmp;
				  }
				}			  
				/* If Element i,i is not equal to 1, multiply the column by 1/i */

				if (dist[sindex+i] != 1) {
				  tmp = galois_single_divide(1, dist[sindex+i], w);
				  srindex = i;
				  for (j = 0; j < rows; j++) {
					dist[srindex] = galois_single_multiply(tmp, dist[srindex], w);
					srindex += cols;
				  }
				}		 
				/* Now, for each element in row i that is not in column 1, you need
				   to make it zero.  Suppose that this is column j, and the element
				   at i,j = e.  Then you want to replace all of column j with 
				   (col-j + col-i*e).   Note, that in row i, col-i = 1 and col-j = e.
				   So (e + 1e) = 0, which is indeed what we want. */
				for (j = 0; j < cols; j++) 
				{
				  tmp = dist[sindex+j];
				  if (j != i && tmp != 0)
				  {
					srindex = j;
					siindex = i;
					for (k = 0; k < rows; k++) 
					{
					  dist[srindex] = dist[srindex] ^ galois_single_multiply(tmp, dist[siindex], w);
					  srindex += cols;
					  siindex += cols;
					}
				  }
				}
			  }
			  
  
  
  
  /* We desire to have row k be all ones.  To do that, multiply
     the entire column j by 1/dist[k,j].  Then row j by 1/dist[j,j]. */

	  sindex = cols*cols;
	  for (j = 0; j < cols; j++) 
	  {
			tmp = dist[sindex];
			if (tmp != 1) { 
			  tmp = galois_single_divide(1, tmp, w);
			  srindex = sindex;
			  for (i = cols; i < rows; i++) {
				dist[srindex] = galois_single_multiply(tmp, dist[srindex], w);
				srindex += cols;
			  }
			}
			sindex++;
	  }

  /* Finally, we'd like the first column of each row to be all ones.  To
     do that, we multiply the row by the inverse of the first element. */

	  sindex = cols*(cols+1);
	  for (i = cols+1; i < rows; i++) 
	  {
		tmp = dist[sindex];
		if (tmp != 1) { 
		  tmp = galois_single_divide(1, tmp, w);
		  for (j = 0; j < cols; j++) dist[sindex+j] = galois_single_multiply(dist[sindex+j], tmp, w);
		}
		sindex += cols;
	  }

  return dist;
}

int *reed_sol_extended_vandermonde_matrix(int rows, int cols, int w)
{
	int *vdm;
	int i, j, k;

	if (w < 30 && (1 << w) < rows) return NULL;
	if (w < 30 && (1 << w) < cols) return NULL;

	vdm = talloc(int, rows*cols);
	if (vdm == NULL) { return NULL; }

	vdm[0] = 1;
	for (j = 1; j < cols; j++) vdm[j] = 0;
	if (rows == 1) return vdm;

	i=(rows-1)*cols;
	for (j = 0; j < cols-1; j++) vdm[i+j] = 0;
	vdm[i+j] = 1;
	if (rows == 2) return vdm;

	for (i = 1; i < rows-1; i++) {
		k = 1;
		for (j = 0; j < cols; j++) {
			vdm[i*cols+j] = k;
			k = galois_single_multiply(k, i, w);
		}
	}
	return vdm;
}

