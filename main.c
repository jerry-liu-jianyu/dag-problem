#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "graph_utils.h"

#define initial  1
#define visited  2
#define finished 3

/*
 * Forward declarations of is_dag
 * See function definition further down for documentation
 */
int is_dag(uint32_t digraph[], int n_vertices);

int DFS(int n, uint32_t digraph[], int n_vertices, int state[]);

int main(int argc, const char **argv)
{
	int i;
	uint32_t *digraph;
        int n_vertices;

	if (argc <= 1) {
		fprintf(stderr, "\nUsage: is_dag <digraph file>+\n\n");
		return 1;
	}

	for (i = 1; i < argc; i++) {
                digraph = NULL;
		n_vertices = 0;
		if (read_graph(argv[i], &digraph, &n_vertices)) {
			fprintf(stderr, "Error reading digraph from file %s\n", argv[i]);
			return 1;
		}

		printf("%s is a dag? %i\n", argv[i], is_dag(digraph, n_vertices));

		free(digraph);
	}

	return 0;
}

/********************************************************************************
 *
 * is_dag
 *
 * Determines whether the given digraph is a dag
 *
 * Given a directed graph and the number of vertices in the graph, returns 1 if
 * the digraph is a dag and 0 otherwise.
 *
 * The digraph is represented as an array of adjacency lists, and each adjacency
 * list is stored as a uint32_t that may be interpreted as a bit-array
 * representation of an adjacency list, with a 1 in bit position i indicates
 * that there is a directed edge to vertex i. Since each adjacency list (i.e.,
 * each uint32_t value of the array) can only represent out edges to vertices 0
 * through 31, the digraph can have at most 32 vertices.
 *
 ********************************************************************************/

int is_dag(uint32_t digraph[], int n_vertices) 
{
  int i; 
  int dag = 1 ;

  int *state;

  state = (int*) malloc(n_vertices * sizeof(int));
  if (state == NULL) {
     printf("Memory not allocated.\n");
     exit(-1);
  }

  for(i=0; i < n_vertices; i++) state[i] = initial;

  /* statr DFS from vertex 0 */
  dag = DFS(0, digraph, n_vertices, state); 

  for ( i = 0; i < n_vertices; i++ ) { 
    if (state[i] == initial )  dag = DFS(i, digraph, n_vertices, state);
    if ( dag == 0 ) break;
  }
  free(state);
  return dag; 
}

int DFS(int n, uint32_t digraph[], int n_vertices, int state[])
{
  int i;
  int dag = 1;

  state[n] = visited;

  for ( i=0; i < n_vertices; i++ ) {
    if ( digraph[n] & ( 1 << i ) ) {
      if (state[i] == initial ) {
        dag = DFS(i, digraph, n_vertices, state);
      }
      else if ( state[i] == visited ) {  
        dag = 0;
        break;     
      }
    }
  }
  state[n] = finished; 
  return dag;
}
