typedef struct node_edges Node_edges;


typedef struct vertex{
    struct vertex *next;
    char *name;
    Node_edges *headedge ;
    int countedge;
} Vertex;

 struct node_edges {
    Node_edges *next;
    char *end;
    int cost;
} ;


typedef struct graph {
    Vertex *head;
    int count;
} WString_graph;




