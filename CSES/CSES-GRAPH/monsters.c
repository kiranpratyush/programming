#include <stdio.h>
#include <limits.h>
#include <string.h>
#include <stdlib.h>

#define MAX_LENGTH 20 // This is for taking input buffer
// Monster problem is a simple traversal problem
// In this first calculate the minimum time will take the monsters to reach the cell
// We can do this by doing the bfs , first in the queue add all the monster initial position
// Then while queue is not empty get each of the position and if the cell is not visited then mark the cell distance of the monster distance
// So in the queue we can put the cell row , cell col , and the initial distance as zero
// After calculating the monster distances then we can start with our current position , to check if we can go to a cell
// check that the cell is not visited,and the time taken to go to that ( current_time+1) should be less than the time taken for any of the monster to come back to the location
// If all this is valid then we can move .
// If the current is the destination keep the parent mapper and build the path. This is same as the previous approach
// while (!(current_row!=parent[current_row] || !(current_col!=parent[current_col])) build the path backwards , then reverse the path
// look out for the or condition: to stop both the row and col should be equal to the parent row and the parent col right hence the or before the not

// This is for the row_col_input format
typedef struct
{
    int row;
    int col;
} row_col_input;
// This is the queue input
typedef struct
{
    row_col_input row_col;
    int distance;
} queue_input;
// Define the queue struct
typedef struct
{
    queue_input *array;
    int front;
    int back;
    int length;
    int capacity;
} Queue;
// Here the capacity will be the total number of cells because in the worst case all the cell data can be inside the queue
void init_queue(Queue *q, int capacity)
{
    q->array = (queue_input *)malloc(capacity * sizeof(queue_input));
    q->front = 0;
    q->back = 0;
    q->length = 0;
    q->capacity = capacity;
}
// Here free the queue
void free_queue(Queue *q)
{
    free(q->array);
    // automatically other things will be freed as they are allocated in the stack;
}
void enqueue(Queue *q, queue_input element)
{
    q->array[q->front] = element;
    q->front = (q->front + 1) % (q->capacity);
    (q->length)++;
}
queue_input deque(Queue *q)
{
    queue_input result = q->array[q->back];
    q->back = (q->back + 1) % (q->capacity);
    (q->length)--;
    return result;
}
// Take input for the first row and the column
row_col_input take_row_col_input()
{
    row_col_input row_col;
    char buffer[MAX_LENGTH];
    fgets(buffer, MAX_LENGTH, stdin);
    // optionally replace the '\n' with the '\0'
    sscanf(buffer, "%d %d", &row_col.row, &row_col.col);
    return row_col;
}
// build the maze
void build_maze(row_col_input row_col, char **map)
{
    for (int i = 0; i < row_col.row; i++)
    {
        for (int j = 0; j < row_col.col; j++)
        {
            char c;
            // This will ignore the '\n' when getting it
            // do while loop will first get the char and if the char is '\n' then the loop will run again.
            do
            {
                c = getchar();
            } while (c == '\n');
            map[i][j] = c;
        }
    }
}
// This is the all possible moves we can do left right top and bottom

row_col_input neigbours[] = {{0, 1}, {0, -1}, {1, 0}, {-1, 0}};

// valid checks

int valid(row_col_input row_col, int row, int col)
{
    return row >= 0 && row < row_col.row && col >= 0 && col < row_col.col;
}
// now update the distance for the monster

void update_min_monster_distance(row_col_input row_col, int **distance, char **map)
{
    // This is for the allocation of pointer of each row
    int **monster_visited = (int **)malloc(row_col.row * sizeof(int *));
    for (int i = 0; i < row_col.row; i++)
    {
        // Now go through each row now allocate the memory initialize with zero for that we can use calloc
        monster_visited[i] = (int *)calloc(row_col.col, sizeof(int));
    }
    Queue monster_queue;
    init_queue(&monster_queue, row_col.row * row_col.col);
    // Now put the row and col index where the monster is present
    for (int i = 0; i < row_col.row; i++)
    {
        for (int j = 0; j < row_col.col; j++)
        {
            if (map[i][j] == 'M')
            {
                queue_input element = {{i, j}, 0};
                enqueue(&monster_queue, element);
                monster_visited[i][j] = 1;
                distance[i][j] = 0;
            }
            else
            {
                distance[i][j] = INT_MAX;
            }
        }
    }

    while (monster_queue.length > 0)
    {
        queue_input current = deque(&monster_queue);
        for (int i = 0; i < 4; i++)
        {
            int new_row = current.row_col.row + neigbours[i].row;
            int new_col = current.row_col.col + neigbours[i].col;

            if (valid(row_col, new_row, new_col) && !monster_visited[new_row][new_col] && map[new_row][new_col] == '.')
            {
                monster_visited[new_row][new_col] = 1;
                queue_input next = {{new_row, new_col}, current.distance + 1};
                enqueue(&monster_queue, next);
                distance[new_row][new_col] = next.distance;
            }
        }
    }

    for (int i = 0; i < row_col.row; i++)
    {
        free(monster_visited[i]);
    }
    free(monster_visited);
    free_queue(&monster_queue);
}
int find_shortest_path(row_col_input row_col, char **map, int **distance, char *shortest_path)
{
    int **visited = (int **)malloc(row_col.row * sizeof(int *));
    queue_input **parent_mapper = (queue_input **)malloc(row_col.row * sizeof(queue_input *));
    for (int i = 0; i < row_col.row; i++)
    {
        visited[i] = (int *)calloc(row_col.col, sizeof(int));
        parent_mapper[i] = (queue_input *)malloc(row_col.col * sizeof(queue_input));
    }

    Queue player_queue;
    init_queue(&player_queue, row_col.row * row_col.col);

    int status = 0;
    for (int i = 0; i < row_col.row; i++)
    {
        for (int j = 0; j < row_col.col; j++)
        {
            if (map[i][j] == 'A')
            {
                queue_input start = {{i, j}, 0};
                enqueue(&player_queue, start);
                visited[i][j] = 1;
                parent_mapper[i][j] = start;
            }
        }
    }

    while (player_queue.length > 0)
    {
        queue_input current = deque(&player_queue);

        if (current.row_col.row == 0 || current.row_col.row == row_col.row - 1 ||
            current.row_col.col == 0 || current.row_col.col == row_col.col - 1)
        {
            status = 1;
            char *path = (char *)malloc(sizeof(char) * row_col.row * row_col.col);
            int path_length = 0;
            queue_input temp = current;
            while (!(temp.row_col.row == parent_mapper[temp.row_col.row][temp.row_col.col].row_col.row &&
                     temp.row_col.col == parent_mapper[temp.row_col.row][temp.row_col.col].row_col.col))
            {
                queue_input parent = parent_mapper[temp.row_col.row][temp.row_col.col];
                if (parent.row_col.row < temp.row_col.row)
                    path[path_length++] = 'D';
                else if (parent.row_col.row > temp.row_col.row)
                    path[path_length++] = 'U';
                else if (parent.row_col.col < temp.row_col.col)
                    path[path_length++] = 'R';
                else
                    path[path_length++] = 'L';
                temp = parent;
            }

            for (int i = 0; i < path_length; i++)
            {
                shortest_path[i] = path[path_length - i - 1];
            }
            shortest_path[path_length] = '\0';
            free(path);
            break;
        }

        for (int i = 0; i < 4; i++)
        {
            int new_row = current.row_col.row + neigbours[i].row;
            int new_col = current.row_col.col + neigbours[i].col;

            if (valid(row_col, new_row, new_col) && !visited[new_row][new_col] &&
                map[new_row][new_col] == '.' && distance[new_row][new_col] > current.distance + 1)
            {
                visited[new_row][new_col] = 1;
                queue_input next = {{new_row, new_col}, current.distance + 1};
                enqueue(&player_queue, next);
                parent_mapper[new_row][new_col] = current;
            }
        }
    }

    for (int i = 0; i < row_col.row; i++)
    {
        free(visited[i]);
        free(parent_mapper[i]);
    }
    free(visited);
    free(parent_mapper);
    free_queue(&player_queue);

    return status;
}

int main()
{
    // freopen("input.txt", "r", stdin);
    // freopen("output.txt", "w", stdout);
    row_col_input row_col = take_row_col_input();
    char **map = (char **)malloc(row_col.row * sizeof(char *));
    int **distance = (int **)malloc(row_col.row * sizeof(int *));
    for (int i = 0; i < row_col.row; i++)
    {
        map[i] = (char *)malloc(row_col.col * sizeof(char));
        distance[i] = (int *)malloc(row_col.col * sizeof(int));
    }
    char *shortest_path = (char *)malloc(row_col.row * row_col.col * sizeof(char));

    build_maze(row_col, map);
    update_min_monster_distance(row_col, distance, map);

    int status = find_shortest_path(row_col, map, distance, shortest_path);
    if (status)
    {
        printf("YES\n");
        printf("%lu\n", strlen(shortest_path));
        printf("%s\n", shortest_path);
    }
    else
    {
        printf("NO\n");
    }

    for (int i = 0; i < row_col.row; i++)
    {
        free(map[i]);
        free(distance[i]);
    }
    free(map);
    free(distance);
    free(shortest_path);
    return 0;
}