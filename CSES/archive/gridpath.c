#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define N 7

// Global variables: visited grid and the move pattern.
bool visited[N][N];
char pattern[100]; // Buffer large enough (grid paths problems use 48 moves).
int patLen;



// Recursive backtracking function.
long long grid_path(int index, int x, int y) {
    if (x == 6 && y == 0)
        return (index == patLen) ? 1LL : 0LL;

    // Pruning checks
    if ((x == 0 || x == 6) && y > 0 && y < 6 && !visited[x][y - 1] && !visited[x][y + 1])
        return 0LL;
    if ((y == 0 || y == 6) && x > 0 && x < 6 && !visited[x - 1][y] && !visited[x + 1][y])
        return 0LL;
    if (x > 0 && x < 6 && y > 0 && y < 6 &&
        ((visited[x - 1][y] && visited[x + 1][y] && !visited[x][y - 1] && !visited[x][y + 1]) ||
         (visited[x][y - 1] && visited[x][y + 1] && !visited[x - 1][y] && !visited[x + 1][y])))
        return 0LL;

    char nextChar = pattern[index];
    long long count = 0;

    if (nextChar == '?') {
        if (y - 1 >= 0 && !visited[x][y - 1]) { // Left
            visited[x][y - 1] = true;
            count += grid_path(index + 1, x, y - 1);
            visited[x][y - 1] = false;
        }
        if (y + 1 < N && !visited[x][y + 1]) { // Right
            visited[x][y + 1] = true;
            count += grid_path(index + 1, x, y + 1);
            visited[x][y + 1] = false;
        }
        if (x + 1 < N && !visited[x + 1][y]) { // Down
            visited[x + 1][y] = true;
            count += grid_path(index + 1, x + 1, y);
            visited[x + 1][y] = false;
        }
        if (x - 1 >= 0 && !visited[x - 1][y]) { // Up
            visited[x - 1][y] = true;
            count += grid_path(index + 1, x - 1, y);
            visited[x - 1][y] = false;
        }
    } else if (nextChar == 'L') {
        if (y - 1 >= 0 && !visited[x][y - 1]) {
            visited[x][y - 1] = true;
            count += grid_path(index + 1, x, y - 1);
            visited[x][y - 1] = false;
        }
    } else if (nextChar == 'R') {
        if (y + 1 < N && !visited[x][y + 1]) {
            visited[x][y + 1] = true;
            count += grid_path(index + 1, x, y + 1);
            visited[x][y + 1] = false;
        }
    } else if (nextChar == 'U') {
        if (x - 1 >= 0 && !visited[x - 1][y]) {
            visited[x - 1][y] = true;
            count += grid_path(index + 1, x - 1, y);
            visited[x - 1][y] = false;
        }
    } else if (nextChar == 'D') {
        if (x + 1 < N && !visited[x + 1][y]) {
            visited[x + 1][y] = true;
            count += grid_path(index + 1, x + 1, y);
            visited[x + 1][y] = false;
        }
    }

    return count;
}


int main() {
    // Redirect standard input and output to files.
    // freopen("input.txt", "r", stdin);
    // freopen("output.txt", "w", stdout);
    
    // Read the move pattern.
    scanf("%s", pattern);
    patLen = strlen(pattern);
    
    // Initialize visited grid.
    for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++)
            visited[i][j] = false;
    visited[0][0] = true;
    
    // Compute and output the number of valid grid paths.
    long long result = grid_path(0, 0, 0);
    printf("%lld\n", result);
    
    return 0;
}
