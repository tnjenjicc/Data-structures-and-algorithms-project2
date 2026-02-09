#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

/*
    1 => Read graph from input file
    2 => Convert file input to correct data structure (Adjacency list or Adjacency matrix)
    3 => Implement either BFS or DFS
    4 => Find 5 most similar words
    5 => Write results to rezultat.txt file
*/

#define MAX 255
#define INF 65535

typedef struct graph
{
    int dimension;
    char **words;
    double **matrix;
} GRAPH;

//https://stackoverflow.com/a/9210560
char** str_split(char* a_str, const char a_delim)
{
    char** result = 0;
    size_t count = 0;
    char* tmp = a_str;
    char* last_comma = 0;
    char delim[2];
    delim[0] = a_delim;
    delim[1] = 0;

    /* Count how many elements will be extracted. */
    while (*tmp)
    {
        if (a_delim == *tmp)
        {
            count++;
            last_comma = tmp;
        }
        tmp++;
    }

    /* Add space for trailing token. */
    count += last_comma < (a_str + strlen(a_str) - 1);

    /* Add space for terminating null string so caller
       knows where the list of returned strings ends. */
    count++;

    result = malloc(sizeof(char*) * count);

    if (result)
    {
        size_t idx  = 0;
        char* token = strtok(a_str, delim);

        while (token)
        {
            assert(idx < count);
            *(result + idx++) = strdup(token);
            token = strtok(0, delim);
        }
        assert(idx == count - 1);
        *(result + idx) = 0;
    }

    return result;
}

//https://stackoverflow.com/a/9210560
double* str_split_double(char* a_str, const char a_delim)
{
    double* result = 0;
    size_t count = 0;
    char* tmp = a_str;
    char* last_comma = 0;
    char delim[2];
    delim[0] = a_delim;
    delim[1] = 0;

    /* Count how many elements will be extracted. */
    while (*tmp)
    {
        if (a_delim == *tmp)
        {
            count++;
            last_comma = tmp;
        }
        tmp++;
    }

    /* Add space for trailing token. */
    count += last_comma < (a_str + strlen(a_str) - 1);

    /* Add space for terminating null string so caller
       knows where the list of returned strings ends. */
    count++;

    result = malloc(sizeof(double) * count);

    if (result)
    {
        size_t idx  = 0;
        char* token = strtok(a_str, delim);

        while (token)
        {
            assert(idx < count);
            *(result + idx++) = strtod(strdup(token), NULL);
            token = strtok(0, delim);
        }
        assert(idx == count - 1);
        *(result + idx) = (0);
    }

    return result;
}

double** floyd(GRAPH g)
{
    double **floyd_matrix = (double**)malloc(g.dimension * sizeof(double*));
    for(int i = 0; i < g.dimension; i++)
        floyd_matrix[i] = (double*)malloc(g.dimension * sizeof(double));

    for(int i = 0; i < g.dimension; i++)
        for(int j = 0 ; j < g.dimension; j++)
        {
            floyd_matrix[i][j] = g.matrix[i][j] > 0 ? g.matrix[i][j] : INF;
            if(i == j)
                floyd_matrix[i][j] = 0;
        }

    for(int i = 0; i < g.dimension; i++)
        for(int j = 0; j < g.dimension; j++)
            for(int k = 0; k < g.dimension; k++)
                if(floyd_matrix[j][k] > floyd_matrix[j][i] + floyd_matrix[i][k])
                    floyd_matrix[j][k] = floyd_matrix[j][i] + floyd_matrix[i][k];

    // printf("\n");
    //  for(int i = 0; i < g.dimension; i++)
    //{
    //     for(int j = 0; j < g.dimension; j++) {
    //         printf("%5.2lf ", floyd_matrix[i][j]);
    //     };
    //     printf("\n");
    // }

    return floyd_matrix;
}

void bfs(int start, GRAPH g)
{
    int visited[g.dimension], queue[g.dimension], front=-1, rear=-1;

    for(int i=0; i<g.dimension; i++)
        visited[i] = 0;

    front++;
    queue[++rear] = start;
    visited[start] = 1;

    while(front<=rear)
    {
        start = queue[front++];
        printf("%s\t", g.words[start]);

        for(int i=0; i<g.dimension; i++)
        {
            if(g.matrix[start][i] > 0 && visited[i] != 1)
            {
                queue[++rear] = i;
                visited[i] =1;
            }
        }
    }

}

void dfs(GRAPH g, int *visited, int start)
{
    printf("%d\t",start+1);
    visited[start]=1;
    for(int i=0; i<g.dimension; i++)
    {
        // printf("%d", visited[i]);
        if(visited[i]!=1 && g.matrix[start][i] > 0)
        {
            // printf("visited: %d", start);
            dfs(g, visited, i);
        }
    }
}

int* getSortedIndexes(double *arr, GRAPH g)
{
    int *indexes = (int*)malloc(g.dimension * sizeof(int));
    double tmp;
    int tmp_index;

    for(int i = 0; i < g.dimension; i++)
        indexes[i] = i;

    for (int i = 0; i < g.dimension; ++i)
        for (int j = i + 1; j < g.dimension; ++j)
            if (arr[i] > arr[j])
            {
                tmp =  arr[i];
                arr[i] = arr[j];
                arr[j] = tmp;

                tmp_index =  indexes[i];
                indexes[i] = indexes[j];
                indexes[j] = tmp_index;
            }

    return indexes;
}

void printSorted(double **matrix, GRAPH g)
{
    FILE *out_file = fopen("rezultat.txt", "w+");
    if (out_file == NULL)
    {
        printf("Error! Could not open file\n");
        exit(-1); // must include stdlib.h
    }
    for(int i=0; i < g.dimension; i++)
    {
        int *sorted = getSortedIndexes(matrix[i], g);
        printf("%s [", g.words[i]);
        fprintf(out_file, "%s [", g.words[i]);
        int count = 0;
        for(int j = 0; j < g.dimension; j++)
        {
            if (matrix[i][j] != INF && matrix[i][j] != 0)
            {
                printf("%s:%.2lf ", g.words[sorted[j]], matrix[i][j] );
                fprintf(out_file, "%s:%.2lf ", g.words[sorted[j]], matrix[i][j] );
                count++;
                if(count > 4)
                    break;
            }
        }
        printf("]\n");
        fprintf(out_file, "]\n");
    }

    fclose(out_file);
}


int main()
{
    char filename[MAX];
    printf("Input file: ");
    scanf("%s", filename);

    FILE *file;
    GRAPH graph;

    file = fopen(filename, "r");
    if (NULL == file)
    {
        printf("file can't be opened \n");
        exit(1);
    }

    char * line = NULL;
    int n;
    int read;

    //read dimension
    read = getline(&line, &n, file);
    graph.dimension = atoi(line);
    printf("%d\n", graph.dimension);
    //read words
    read = getline(&line, &n, file);
    graph.words = str_split(line, ' ');
    //read matrix
    int row = 0;
    graph.matrix = malloc(sizeof(double*) * graph.dimension);
    while ((read = getline(&line, &n, file)) != -1)
    {
        graph.matrix[row++] = str_split_double(line, ' ');
    }
    //print matrix
    for(int i = 0; i < graph.dimension; i++)
    {
        for(int j = 0; j < graph.dimension; j++)
        {
            printf("%.2lf ", graph.matrix[i][j]);
        };
        printf("\n");
    }
    //root entry
    int root;
    do
    {
        printf("Enter root index: ");
        scanf("%d", &root);
    }
    while(root < 0 || root >= graph.dimension);

    printf("BFS: \n");
    bfs(root, graph);
    //dfs(graph, visited, 11);
    printf("\n\n\n");

    double **floyd_matrix = floyd(graph);
    printf("\n\n\n");
    printSorted(floyd_matrix, graph);

    //dfs
    printf("\n");
    // int *visited = (int*)malloc(graph.dimension * sizeof(int));
    // dfs(graph, visited, 11);
    // bfs(graph, 11);

    // Closing the file
    fclose(file);
    free(graph.matrix);

    return 0;
}
