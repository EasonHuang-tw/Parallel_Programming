#include "page_rank.h"

#include <stdlib.h>
#include <cmath>
#include <omp.h>
#include <utility>

#include "../common/CycleTimer.h"
#include "../common/graph.h"

// pageRank --
//
// g:           graph to process (see common/graph.h)
// solution:    array of per-vertex vertex scores (length of array is num_nodes(g))
// damping:     page-rank algorithm's damping parameter
// convergence: page-rank algorithm's convergence threshold
//
void pageRank(Graph g, double *solution, double damping, double convergence)
{

  // initialize vertex weights to uniform probability. Double
  // precision scores are used to avoid underflow for large graphs
	
  int numNodes = num_nodes(g);
	double *solution_old = (double*)malloc(sizeof(double) * g->num_nodes);;
	int *no_outgoing_list = (int*)malloc(sizeof(int) * g->num_nodes);;
	double converged_num = 1000;

	double equal_prob = 1.0 / numNodes;
  
	for (int i = 0; i < numNodes; ++i)
  {
    solution[i] = equal_prob;
  }

	int counter=0;
	for (int i = 0; i<numNodes; ++i)
	{
			if(outgoing_size(g,i) == 0){
				no_outgoing_list[counter] = i;
				counter ++;
			}

	}

	
	while(converged_num > convergence){
					converged_num = 0;
					for (int i=0; i<numNodes; i++){
						solution_old[i] = solution[i]; 
						solution[i] = 0;	
					}

					for (int i=0; i<num_nodes(g); i++) {

						// Vertex is typedef'ed to an int. Vertex* points into g.outgoing_edges[]
						const Vertex* start = incoming_begin(g, i);
						const Vertex* end = incoming_end(g, i);
						//printf("Edge %u size %u\n",i,incoming_size(g,i));
						for (const Vertex* v=start; v!=end; v++){
							//printf("Edge %u %u\n", i,*v);
							//printf("Edge %u outgoing %u\n", i, outgoing_size(g,*v)); 
							solution[i] += solution_old[*v] / outgoing_size(g,*v);
						}
						solution[i]	= damping * solution[i] + (1.0-damping) / (double)numNodes;
					 
						for (int j = 0; j<counter; j++){
								solution[i] += solution_old[no_outgoing_list[j]] / numNodes* damping;
								//printf("im here");
							
							
						}
					
						converged_num += abs(solution_old[i]-solution[i]);
					}
					
			printf("converged num: %f\n", converged_num);
	}
	
  /*
     For PP students: Implement the page rank algorithm here.  You
     are expected to parallelize the algorithm using openMP.  Your
     solution may need to allocate (and free) temporary arrays.

     Basic page rank pseudocode is provided below to get you started:

     // initialization: see example code above
     score_old[vi] = 1/numNodes;

     while (!converged) {

       // compute score_new[vi] for all nodes vi:
       score_new[vi] = sum over all nodes vj reachable from incoming edges
                          { score_old[vj] / number of edges leaving vj  }
       score_new[vi] = (damping * score_new[vi]) + (1.0-damping) / numNodes;

       score_new[vi] += sum over all nodes v in graph with no outgoing edges
                          { damping * score_old[v] / numNodes }

       // compute how much per-node scores have changed
       // quit once algorithm has converged

       global_diff = sum over all nodes vi { abs(score_new[vi] - score_old[vi]) };
       converged = (global_diff < convergence)
     }

   */
}
