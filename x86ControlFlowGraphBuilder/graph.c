#include <stdio.h>
#include <stdlib.h>

#include "graph.h"

/* clean graph structure */
void clean_graph (p_graph graph) {

	graph->start            = NULL;
	graph->end              = NULL;
    graph->lookup_table     = NULL;
	graph->len_lookup_table = 0;

	return;
}

/* Free structure graph */
void free_graph (p_graph graph) {

	p_node_list tmp_list, list;

	list = graph->start;
	if (list != NULL) {

		while (list->next != NULL) {
			zpn_free_str (list->node.instr);
			tmp_list = list;
			list = list->next;
			free (tmp_list);
		}

		zpn_free_str (list->node.instr);
		free (list);
	}

	free (graph->lookup_table);
	graph->lookup_table = NULL;
	graph->start        = NULL;
	graph->end          = NULL;
	return;
}

/* Add child to node */
void add_child_to_node (p_node node, int child) {

	if (node->left == -1) node->left = child;
	else if (node->right == -1) node->right = child;
	else {
		fprintf (stderr, "GRAPH ERROR : NODE HAVE YET TWO CHILDREN !!\n");
		fprintf (stderr, "NODE %d\nCHILD 1 : %X\nCHILD 2 %X\nCHILD ON %X\n", 
				node->id, node->left, node->right, child);
		exit (EXIT_FAILURE);
	}

	return;
}

/* Return new allocated list of node properly initialized */
p_node_list new_node_list (p_graph graph, int father, int child) {

	p_node_list list = NULL;
	p_node node = NULL;

	list = (p_node_list) calloc (1, sizeof (t_node_list));
	if (list == NULL) {
		fprintf (stderr, "%s:%d error with calloc\n", __FILE__, __LINE__);
		exit (EXIT_FAILURE);
	}

	list->next = NULL;
	node = &(list->node);
	node->id = father;
	node->left = -1;
	node->right = -1;

	if (father >= graph->len_lookup_table) {
		fprintf (stderr, "%s:%d father >= len_lookup_table\n", __FILE__, __LINE__);
		free (list);
		exit (EXIT_FAILURE);
	}

    graph->lookup_table[father] = list;

	if (child >= 0) {
		add_child_to_node (node, child);
	}

	if (graph->end != NULL) graph->end->next = list;
	if (graph->start == NULL) graph->start = list;
    graph->end = list;
	return list;
}


/* Search node, return node if present NULL else */
p_node search_node (p_graph graph, int id) {

	if (id >= graph->len_lookup_table) {
		fprintf (stderr, "%s:%d id[%d] >= len_lookup_table[%d]\n", 
		  __FILE__, __LINE__, id, graph->len_lookup_table);
		exit (EXIT_FAILURE);
	}

	if (graph->lookup_table[id] == NULL)
		return NULL;

	return &(graph->lookup_table[id]->node);
}

/* Add string to node using memory to save last position of last string */
int add_str_to_node (p_node node, char *str) {

	static p_zpn_str *next = NULL;
	static p_node old_node = NULL;

	if (old_node != node || node == NULL) {

		old_node = node;
		if (node == NULL) return 0;

		next = &(node->instr);
	} 

	next = zpn_add_str (next, str);
	if (next == NULL) return 0;
	
	return 1;
}

/* Add node to list                      * 
 * ------------------------------------- *
 * If child < 0, don't add child to node */
p_node add_node_to_list (p_graph graph, int father, int child, char *str) {

	p_node node = NULL;
	p_node_list list = NULL;

	list = graph->lookup_table[father];
	if (list == NULL) {

		list = new_node_list (graph, father, child);
		if (list == NULL) return NULL;
		list->node.instr = NULL;

	} 

	node = &(list->node);
	if (child >= 0)
		add_child_to_node (node, child);

	if (str != NULL)
		if (!add_str_to_node (node, str))
			return NULL;

	return &(list->node);
}

/* Write directed graph in file for GraphViz Format */
void write_directed_graph (FILE *fd, p_graph graph) {

	p_node node = NULL;
	p_node_list list = graph->start;

	fprintf (fd, "digraph G {\n");
	while (list != NULL) {

		node = &(list->node);

		fprintf (fd, "\nline %d -> {", node->id);

		if (node->left >= 0) fprintf (fd, "line %d;", node->left);
		if (node->right >= 0) fprintf (fd, "line %d;", node->right);

		fprintf (fd, "}\n");
		list = list->next;
	}

	fprintf (fd, "}\n");
	return;
}

/* Transfer node1 to node2, node2 take children of node1 and node2 *
 * become child of node1                                           */
/* TODO update structure due to instr in node
void transfer_branch (p_graph graph, int id1, int id2) {

	p_node node1, node2;

	node1 = search_node (graph, id1);
	if (node1 == NULL) {
		fprintf (stderr, "GRAPH ERROR : TRY TO TRANSFER ON A UNKNOWN NODE !! %X\n", id1);
		exit (EXIT_FAILURE);
		return;
	}

	node2 = add_node_to_list (graph, id2, -1, NULL); 
	node2->left = node1->left;
	node2->right = node1->right;
	node1->left = node2->id;
	node1->right = -1;

	return;
}
*/
