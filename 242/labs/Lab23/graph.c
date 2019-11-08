#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "graph.h"
#include "queue.h"
#include "mylib.h"
#include <limits.h>

#define IS_UNVISITED(x) (UNVISITED == (x).state)

#define IS_VISITED_SELF(x) (VISITED_SELF == (x).state)

typedef enum { UNVISITED, VISITED_SELF, VISITED_DESCENDANTS } state_t;

typedef struct Vertex* vertex;

int step;

struct Vertex{
	int dis; /*distance*/
	state_t state;
	int pre; /*predicesor*/
	int finish;
};


struct Graphrec {
 
	vertex ver; 
	int size; /*number of vertices*/
	int** edges;
};


graph graph_new(int num_vertices){
	/*a fuctuion that crates a new graph.*/

		int i, j;

	/*allocate memory for graph*/	
	graph g =  emalloc(sizeof *g);
	g->size = num_vertices;
	g->edges = emalloc(g->size * sizeof(*g->edges));
	for(i = 0; i < g->size; i++){
	  g->edges[i] = emalloc(g->size * sizeof(**g->edges));
	}
 	g->ver = emalloc(g->size * sizeof(*g->ver));

	/*initilize var to 0*/
	for(i = 0; i < g->size; i++){
		
	  for(j = 0; j < g->size; j++) g->edges[i][j] = 0;

		g->ver[i].dis = INT_MAX;	
		g->ver[i].finish = 0; 
		g->ver[i].state = UNVISITED; 
		g->ver[i].pre = -1; 
	} 

return g;
}


int graph_add_edge(graph g, int u, int v, int is_bi){
	/*adds the the edege to the gaph from u -> v, in is_bi is 1 
  		the it makes it bi directonal, adding u <- v  as well */
	
	g->edges[u][v] = 1;
	if(is_bi) g->edges[v][u] = 1;

	return EXIT_SUCCESS;
}



int graph_bfs(graph g, int source){
	
	queue q;
	q = queue_new();
	
	g->ver[source].state = VISITED_SELF;
	g->ver[source].dis = 0;
	
	enqueue(q, source);
	while(queue_size(q) > 0){
		int dequeued;
		int i;
		queue_print(q);
		dequeued = dequeue(q);
		printf("dequeued: %d\n",dequeued);
		for(i = 0; i < g->size; i++){
			if((g->edges[dequeued][i] == 1) && (IS_UNVISITED(g->ver[i]))){
				
				g->ver[i].state = VISITED_SELF;
				g->ver[i].dis = g->ver[dequeued].dis + 1;
				g->ver[i].pre = dequeued;
				enqueue(q, i);
			}
		}
		g->ver[dequeued].state = VISITED_DESCENDANTS; 
	}
	return EXIT_SUCCESS;
}

int visit(int v, graph g){
	int i;
	g->ver[v].state =  VISITED_SELF;
	step++;
	g->ver[v].dis = step;

	for(i = 0; i < g->size; i++){

		if(g->edges[v][i] == 1){

			if(IS_UNVISITED(g->ver[i])){
				g->ver[i].pre = v;
				visit(i, g);
			}
		}
	}
	step++;
	g->ver[v].state = VISITED_DESCENDANTS;
	g->ver[v].finish =  step;
			
	return EXIT_SUCCESS;
}


int graph_dfs(graph g){
	int i;
	
	for(i = 0; i < g->size; i++){
		g->ver[i].state = UNVISITED; 
		g->ver[i].pre = -1; 
		g->ver[i].finish = 0;
		g->ver[i].dis= -1;
	}	

	step = 0;
for (i = 0; i < g->size; i++){
		if(IS_UNVISITED(g->ver[i])) visit(i, g);
			
		
	}
	return EXIT_SUCCESS;
	
}

void graph_free(graph g){
	int i;

	free(g->ver);
	for(i = 0; i < g->size; i++) free(g->edges[i]);
	free(g->edges);
	free(g);
}



void graph_print(graph g){
	int i, j;

	printf("____graph:___\n");
	for(i = 0; i < g->size; i++){
		printf("%d | ", i); 
		
		for(j = 0; j < g->size; j++){
			if(g->edges[i][j] == 1) printf("%d, ", j);
		}
		printf("\n");

	}

	graph_print_bfs(g);

}

void graph_print_bfs(graph g){
	int i;

	printf("____DFS:___\n");
	printf("Vertex | Distance | Predecesor | finish\n");
	for(i = 0; i < g->size; i++){
		
		printf("%2d  %7d %12d %12d \n", i, g->ver[i].dis, g->ver[i].pre, g->ver[i].finish); 

	}

}


