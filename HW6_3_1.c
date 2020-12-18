//
#include <stdio.h>
#include "minheap.h"
#include "unionfind.h"
#define MAX_VERTICES 100
#define INF 9999
typedef struct GraphType {
	int n;					// ������ ����
	int adj_mat[MAX_VERTICES][MAX_VERTICES];
} GraphType;
// �׷��� �ʱ�ȭ 
void graph_init(GraphType *g)
{
	int r, c;
	g->n = 0;
	for (r = 0; r < MAX_VERTICES; r++)
		for (c = 0; c < MAX_VERTICES; c++)
			g->adj_mat[r][c] = INF;
}
void read_graph(GraphType *g, char *filename)
{
	int number, u, v, weight;
	FILE *fp;
    fp = fopen(filename, "rt");
	if (fp == NULL) 
	{
		printf("file %s open error!\n", filename);
		return;
	}
     // �ڵ� ����
	fscanf(fp, "%d", &number);
	g->n = number;

	while (fscanf(fp, "%d %d %d", &u, &v, &weight) != EOF) {
		g->adj_mat[u][v] = weight;
		g->adj_mat[v][u] = weight;
	}
    fclose(fp);
}
void write_graph(GraphType *g, char *filename)
{
	int i, j;
    FILE *fp;
     
    if (filename == NULL) fp = stdout;
    else {
        fp = fopen(filename, "wt");
	    if (fp == NULL) 
	    {
		    printf("file %s open error!\n", filename);
	        return;
	    }
    }
	fprintf(fp, "%d\n", g->n);

	for (i = 0; i < g->n; i++) 
		for (j = i; j < g->n; j++) 
			if (g->adj_mat[i][j] != INF)
				fprintf(fp, "%d %d %d\n", i, j, g->adj_mat[i][j]);

	if (filename != NULL) fclose(fp);
}
void insert_heap_edge(HeapType *h, int u, int v, int weight)
{
	element e;
	e.u = u;
	e.v = v;
	e.key = weight;
	insert_min_heap(h, e); 
}
// ���� ����̳� ���� ����Ʈ���� �������� �о �ּ� ������ ���� 
// ����� ���� �׷����� �������� �����Ѵ�.
void insert_all_edges(HeapType *h, GraphType *g)
{
	int u, v;

	for (u = 0; u < g->n; u++)
		for (v = 0; v < g->n; v++)
			if (g->adj_mat[u][v] != INF)
				insert_heap_edge(h, u, v, g->adj_mat[u][v]);
}
// kruskal�� �ּ� ��� ���� Ʈ�� ���α׷�
void kruskal(GraphType *g, GraphType *t)
{
	int edge_accepted = 0; // ������� ���õ� ������ ��
	HeapType h; // �ּ� ����
	int uset, vset; // ���� u �� ���� v �� ���� ��ȣ
	element e; // ���� ���

	init(&h); // ���� �ʱ�ȭ
	insert_all_edges(&h, g); // ������ �������� ����
	set_init(g->n); // ���� �ʱ�ȭ
	while (edge_accepted < (g->n - 1)) // ������ �� < (n-1)
	{
		e = delete_min_heap(&h); // �ּ� �������� ����: ���� key(?weight)�� ���� ����
		uset = set_find(e.u); // ���� u �� ���� ��ȣ
		vset = set_find(e.v); // ���� v �� ���� ��ȣ
		if (uset != vset) { // ���� ���� ������ �ٸ���, �� ����Ŭ�� ������ ������
			t->adj_mat[e.u][e.v] = e.key;
			printf("(%d, %d) %d \n", e.u, e.v, e.key);
			edge_accepted++;
			set_union(uset, vset); // �ΰ��� ������ ��ģ��.
		}
	}
}
main()
{
	GraphType g, t;		// �Է� �׷���, ��� Ʈ��
	
	graph_init(&g);
	//read_graph(&g, "input1.txt");
	read_graph(&g, "input2.txt");

	graph_init(&t);
	t.n = g.n;
	
	kruskal(&g, &t);
	
	printf("\n���Ϸ� ���:\n");
	write_graph(&t, "output.txt");	
	write_graph(&t, NULL);	// to stdout
}