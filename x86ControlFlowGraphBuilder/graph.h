#ifndef GRAPH_H
#define GRAPH_H

#include "../C-resources/ZpnString.h"

/* Basic node structure for Directed Graph */
typedef struct _node {

	int id; /* absolute position in file */
	int left, right;
	p_zpn_str instr; /* All instructions contained in node */

} t_node, *p_node;

/* Chained list of nodes */
typedef struct node_list {

	t_node           node;
	struct node_list *next;

} t_node_list, *p_node_list;

/* Graph of nodes */
typedef struct graph {

	struct node_list **lookup_table;
    struct node_list *start;
    struct node_list *end;

	int len_lookup_table;

} t_graph, *p_graph;

/* Return new allocated list of node properly initialized */
p_node_list new_node_list (p_graph graph, int father, int child);

/* Add child to node */
void add_child_to_node (p_node node, int child);

/* Free structure graph */
void free_graph (p_graph graph);

/* Add node to list                      * 
 * ------------------------------------- *
 * If child < 0, don't add child to node *
 * If str == NULL, don't add str to node */
p_node add_node_to_list (p_graph, int father, int child, char *str);

/* Search node, return node if present NULL else */
p_node search_node (p_graph graph, int id);

/* Transfer node1 to node2, node2 take children of node1 and node2 *
 * become child of node1                                           *
 * TODO SEE graph.c
void transfer_branch (p_graph graph, int id1, int id2);
*/

/* Clean graph structure */
void clean_graph (p_graph graph);

/* Write directed graph in file for GraphViz Format */
void write_directed_graph (FILE *fd, p_graph graph);

/* Add string to node using memory to save last position of last string */
int add_str_to_node (p_node node, char *str);

#endif

