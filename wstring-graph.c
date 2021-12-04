#include <malloc.h>
#include "stdio.h"
#include "wstring-graph.h"
#include "string.h"
#include "graph-utils.h"

/*
 * This is a program that manipulates the elemenets
 * of a graph. A graph is consisted of vertices that
 * contain name and have edges to other vertices.
 * Using this program we can add vertex and edge and get
 * the count of the verties and also sort the elements in a
 * graph in dictionary order. The code comments are ABOVE the
 * functions and it explains what the code does.
 */
static Vertex * searching(WString_graph
                          const * graph,
                          const char namesearch[]);
static Node_edges * searchingedge(Vertex * namever,
                                  const char nameend[]);
static char ** sort_dic_order(char ** array, int count);
int getsizemalloc(char ** arr);

/*
 * This function simply initialize the
 * graph to null and set the total numbers of
 * vertices to zero
 */
void init_graph(WString_graph *
const graph) {
    if (graph != NULL) {
        graph -> head = NULL;
        graph -> count = 0;
    }
}
/*
 * This function add a new vertex to the graph
 * and set the name of the vertix to the given
 * name that is passed to the function and return
 * 1 if its added correctly and 0 zero if it is not
 * added correctly.
 */
int new_vertex_add(WString_graph *
const graph,
                   const char new_vertex[]) {
    Vertex * new_ver;
    if (graph == NULL) {
        return 0;
    }
    if (new_vertex == NULL) {
        return 0;
    }

    if (is_existing_vertex(graph, new_vertex) == 1) {
        return 0;
    }
    /*
     * if special cases didnt happen. then the
     * vertex will allocate enough memeory to store a vertex
     * then the name will allocate the memeory to the length of
     * the name passed then the name will be copied into the
     * field of the vertex. The vertex is getting added to the
     * front of the graph so the next element of the vertex will
     * be the head of the graph and the new head will be new vertex
     * Also the headedge is set to null also because no edge yet is
     * added.
     */
    new_ver = malloc(sizeof(Vertex));
    if (new_vertex == NULL) {
        return 0;
    }

    /*
     * the length of the string passed should be plus one becuase
     * starts at zero.
     */
    (new_ver -> name) = malloc((sizeof(char)) * strlen(new_vertex) + 1);
    strcpy((new_ver -> name), new_vertex);
    new_ver -> headedge = NULL;
    new_ver -> next = graph -> head;
    new_ver -> countedge = 0;
    graph -> head = new_ver;
    graph -> count++;

    return 1;
}
/*
 * This function simply returns the number of
 * vertices.
 */
int num_vertices(const WString_graph *
const graph) {
    if (graph == NULL) {
        return 0;
    }
    return graph -> count;
}
/*
 * This function ckecks to see if the vertiex with given
 * name exists in the graph given. First a temp variable
 * is created to store the head of the graph so we can iterate
 * from it. So we should check the head variable until we reach
 * the element null to stop. Then we set the head to next element.
 */
int is_existing_vertex(const WString_graph *
const graph,
                       const char name[]) {
    Vertex * temp;
    if (graph == NULL) {
        return 0;
    }
    temp = graph -> head;
    while (temp != NULL) {
        if (strcmp((temp -> name), name) == 0) {
            return 1;
        } else {
            temp = temp -> next;
        }
    }
    return 0;

}
int add_edge(WString_graph *
const graph,
             const char source[],
             const char dest[], int cost) {
    Node_edges * temp;
    Vertex * source1;
    Node_edges * newedge;
    if (graph == NULL) {
        return 0;
    }
    if (cost < 0) {
        return 0;
    }
    /*
     * if both the vertix and the edge exist we will
     * extract them and then set the name of the newedge
     * to the end field and then set the cost to the given
     * variable, then the next element of the new edge to the head
     * and then the head to new edge to add to the front. Then increment
     * the number of edges.
     */
    if (is_existing_vertex(graph, source) == 1 &&
        is_existing_vertex(graph, dest) == 1) {

        source1 = searching(graph, source);
        temp = searchingedge(source1, dest);
        if (temp != NULL) {
            temp -> cost = cost;
        } else {
            newedge = (Node_edges * ) malloc(sizeof(Node_edges));
            if (newedge == NULL) {
                return 0;
            }
            newedge -> end = (char * ) malloc(strlen(dest) + 1);

            strcpy(newedge -> end, dest);
            newedge -> cost = cost;
            newedge -> next = source1 -> headedge;
            source1 -> headedge = newedge;
            source1 -> countedge++;
        }
    }
    /*
     * if neither the source nor the edge exist. We should add them
     * first. and then new edge will be dynamically allocating memory
     * then the name will be copied into the fields variable. Then the cot
     * will be set. Then we get the vertix source and set the next line to NULL
     * since this is the first element of the list. and then set the headedge
     * to the new edge.
     *
     */
    if (is_existing_vertex(graph, source) == 0 &&
        is_existing_vertex(graph, dest) == 0) {

        new_vertex_add(graph, source);
        if (strcmp(source, dest) != 0) {
            new_vertex_add(graph, dest);
        }

        newedge = (Node_edges * ) malloc(sizeof(Node_edges));
        newedge -> end = (char * ) malloc(strlen(dest) + 1);
        strcpy(newedge -> end, dest);
        newedge -> cost = cost;

        source1 = searching(graph, source);
        newedge -> next = NULL;
        source1 -> headedge = newedge;
        source1 -> countedge++;

    }

    /*
     * If the vertiex starting exists but the dest doesnt.then we need to
     * add the dest vertix then set the name and cost of it. Then get the
     * source and set the next to head edge then the head to new to add it
     * to the front.
     */
    if (is_existing_vertex(graph, source) == 1 &&
        is_existing_vertex(graph, dest) == 0) {

        new_vertex_add(graph, dest);

        newedge = (Node_edges * ) malloc(sizeof(Node_edges));
        newedge -> end = (char * ) malloc(strlen(dest) + 1);
        strcpy(newedge -> end, dest);
        newedge -> cost = cost;

        source1 = searching(graph, source);

        newedge -> next = source1 -> headedge;
        source1 -> headedge = newedge;
        source1 -> countedge++;

    }

    /*
     * IF the source vertex doesnt exist and the dest does
     * we add the source and do the same we did above.
     */
    if (is_existing_vertex(graph, source) == 0 &&
        is_existing_vertex(graph, dest) == 1) {

        new_vertex_add(graph, source);

        newedge = (Node_edges * ) malloc(sizeof(Node_edges));
        newedge -> end = (char * ) malloc(strlen(dest) + 1);
        strcpy(newedge -> end, dest);
        newedge -> cost = cost;

        source1 = searching(graph, source);

        newedge -> next = NULL;
        source1 -> headedge = newedge;
        source1 -> countedge++;
    }
    return 1;
}
/*
 * This function searches if the given name is in the vertix
 * then it will return the VERTIX. This functuon is used in the
 * add vertix function above. Functionality is the same as the
 * is_existence_function but instead the Vertix will be returned.
 * IF the vertex is not present then Null is returned
 */
Vertex * searching(WString_graph
                   const * graph,
                   const char namesearch[]) {
    Vertex * temp = graph -> head;
    while (temp != NULL) {
        if (strcmp(temp -> name, namesearch) == 0) {
            return temp;
        } else {
            temp = temp -> next;
        }
    }
    return NULL;
}

/*
 * This function just returns the edge that is in the vertix by
 * getting the name of the destination and the vertex. (The vertex itself
 * is the starting point.
 */

Node_edges * searchingedge(Vertex * namever,
                           const char nameend[]) {
    Node_edges * temp = namever -> headedge;
    while (temp != NULL) {
        if (strcmp(temp -> end, nameend) == 0) {
            return temp;
        } else {
            temp = temp -> next;
        }
    }
    return NULL;

}

/*
 * This function gets the weight of each edge that is
 * in the graph by getting the src and the dest of the
 * vertex. If the vertex and the edge are both existing
 * in the graph then we get the source that is in the graph
 * then if the temp is not null we need to store the cost
 * passed to the function to the field of the edge.
 * If this operation is not done properly then -1 will be
 * returned.
 */
int get_weight_of_edge(const WString_graph *
const graph,
                       const char source[],
                       const char dest[]) {
    Node_edges * temp;
    Vertex * vertemp;

    if (is_existing_vertex(graph, source) == 0 ||
        is_existing_vertex(graph, dest) == 0 || source == NULL ||
        dest == NULL) {
        return -1;
    }

    if (is_existing_vertex(graph, source) == 1 &&
        is_existing_vertex(graph, dest) == 1) {

        vertemp = searching(graph, source);
        temp = searchingedge(vertemp, dest);
        if (temp != NULL) {
            return temp -> cost;
        } else {
            return -1;
        }
    }

    return -1;
}
/*
 * This function will return the number of neighbors that
 * a vertex has. We have a variable called countedge in
 * each ege that is created so we just need to return it
 * if the vertex and the edge exist in the graph given.
 */
int num_neighbors(const WString_graph *
const graph,
                  const char vertex[]) {
    Vertex * temp;
    if (graph == NULL) {
        return -1;
    }
    if (searching(graph, vertex) == NULL) {
        return -1;
    }
    if (vertex == NULL) {
        return -1;
    }
    temp = searching(graph, vertex);
    return temp -> countedge;

}

/*
 * This function extracts the vertices from the graph
 *  and stores it in an array in dictionary order.
 *  First the number of vertices is obtained in order to
 *  set the size of the array to be dynamically allocated.
 *  then the variable temp is created to store the graph head
 *  then looping through the graph and getting the vertices name
 *  is happen and store dynamically. After that the sort function
 *  is called and to sort the array in dictionary order.
 */

char ** get_vertices(const WString_graph *
const graph) {
    char * temp_name;
    int count = num_vertices(graph);
    int index = 0;
    char ** array = (char ** ) malloc((count + 1) * sizeof(char * ));
    Vertex * temp = graph -> head;

    if (graph == NULL) {
        return NULL;
    }
    while (temp != NULL) {
        temp_name = (char * ) malloc(strlen(temp -> name) + 1);
        strcpy(temp_name, temp -> name);
        array[index] = temp_name;
        index++;
        temp = temp -> next;

    }
    array[index] = NULL;

    return sort_dic_order(array, count);

}

/*
 *  This function simply loops through out the
 *  array and returns when the array reaches NULL
 *  because the last element is set to be NULL.
 */
int getsizemalloc(char ** arr) {
    int i = 0;
    if (arr != NULL) {
        while (arr[i] != NULL) {
            i++;
        }
    }
    return i + 1;

}

/*
 * This function sorts the array by comparing the characters
 * together and then swaps the ones that are not in order.
 */

char ** sort_dic_order(char ** array, int count) {
    int size = count;
    int i;
    int j;
    char ** temp = (char ** ) malloc(sizeof(char * ));
    for (i = size; i > 0; i--) {
        for (j = 0; j <= i - 2; j++) {
            /*
             *swapping strings if they are not in the lexicographical order
             */
            if (strcmp(array[j], array[j + 1]) > 0) {
                temp[0] = array[j];
                array[j] = array[j + 1];
                array[j + 1] = temp[0];
            }
        }
    }
    free(temp);
    return array;
}
/*
 * This function frees each element of an array of names
 * simply using a for loop. and at the ent removes the
 * name variable.
 */
void free_vertex_list(char **
const names) {
    if (names != NULL) {
        int i;
        int size = getsizemalloc(names);
        for (i = 0; i < size; i++) {
            free(names[i]);
        }
        free(names);
    }

}

/*
 * This function is used to destroy and deallocate all the
 * memory used by graph. It checks if the graph `s size
 * is more than 0 and then goes through a while loop that
 * calls the remove vertext function because the that
 * function automatically removes all the vertexs that end
 * in that too.
 */
void destroy_graph(WString_graph *
const graph) {
    int size = graph -> count;
    Vertex * temp;

    if (graph != NULL) {
        if (size != 0) {
            temp = graph -> head;
            while (temp != NULL) {
                remove_vertex(graph, temp -> name);
                temp = graph -> head;
            }
            graph -> count = 0;

        }
    }
}
/*
 * This function will remove an edge between the soruce and the
 * dest so first we find the source and the edge then we then if
 * the edge exists we define a prev node in order to connect the
 * edges together because we dont know where exactly is the edge.
 *
 */
int remove_edge(WString_graph *
const graph,
                const char source[],
                const char dest[]) {
    Node_edges * prev, * deletedge;
    Vertex * temp = searching(graph, source);
    if (temp != NULL) {
        Node_edges * temp_edge = searchingedge(temp, dest);
        /* if the edge is not null prev is
         * set to the temp headedge
         */
        if (temp_edge != NULL) {
            prev = temp -> headedge;
            if (strcmp(prev -> end, dest) == 0) {
                temp -> headedge = prev -> next;
                free(prev -> end);
                free(prev);
            }
                /* if dest is not equal to the name of
                 * the edge then we need to iterate through
                 * the list and then remove it.
                 */
            else {
                prev = temp -> headedge;
                while (strcmp(prev -> next -> end, dest) != 0) {
                    prev = prev -> next;
                }
                deletedge = prev -> next;
                prev -> next = deletedge -> next;
                free(deletedge -> end);
                free(deletedge);
            }

            temp -> countedge--;
            return 1;
        }
        return 0;
    }

    return 0;

}

/*
 * This function removes the vertex given in the graph. If the ver
 * -tex is removed all the edges that are incoming or outgoing should
 * be removed too. So the function remove edge is called here too.
 *
 */
int remove_vertex(WString_graph *
const graph,
                  const char vertex[]) {
    int i;
    int size;
    Vertex * temp, * temp2, * temp3, * temp4;
    Node_edges * tempedge;

    if (graph == NULL) {
        return 0;
    }
    temp = searching(graph, vertex);
    if (temp != NULL) {
        size = temp -> countedge;
        if (size != 0) {
            for (i = 0; i < size; i++) {
                tempedge = temp -> headedge;
                remove_edge(graph, temp -> name, tempedge -> end);
            }
            temp -> countedge = 0;

        }
        temp2 = graph -> head;
        for (i = 0; i < temp2 -> countedge; i++) {
            remove_edge(graph, temp2 -> name, vertex);
            temp2 = temp2 -> next;

        }
        /*
         * Removing the vertex itself starts here.
         */
        temp3 = graph -> head;
        if (strcmp(temp3 -> name, vertex) == 0) {
            graph -> head = temp3 -> next;
        } else {
            /*
             * while loop to compare vertex name
             * and our temp`s name. if they are
             * not the same we move to the next one
             * until we get to it.
             */
            while (strcmp(temp3 -> next -> name, vertex) != 0) {
                temp3 = temp3 -> next;
            }
            temp4 = temp3 -> next;
            temp3 -> next = temp4 -> next;
        }

        free(temp -> name);
        free(temp);
        graph -> count--;
        return 1;
    }
    return 0;

}
/*
 * This function uses the same logic as the get_vertex
 * it stores the neighbors of a given vertex into an
 * array and then sort it in dic order and then returns it.
 */
char ** get_neighbors(const WString_graph *
const graph,
                      const char vertex[]) {
    Vertex * vertex1 = searching(graph, vertex);
    char * temp_name;
    int count = vertex1 -> countedge;
    int index = 0;
    /*
     * Dynamically memory allocated array.
     */
    char ** array = (char ** ) malloc((count + 1) * sizeof(char * ));
    Node_edges * temp = vertex1 -> headedge;
    if (graph == NULL || vertex1 == NULL) {
        return NULL;
    }
    while (temp != NULL) {
        /*
         * Dynamically memory allocated array.
         */
        temp_name = (char * ) malloc(strlen(temp -> end) + 1);
        strcpy(temp_name, temp -> end);
        array[index] = temp_name;
        index++;
        temp = temp -> next;

    }
    array[index] = NULL;
    return sort_dic_order(array, count);

}
