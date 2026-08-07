#include <bits/stdc++.h>
using namespace std;

int main()
{
    // freopen("input.txt", "r", stdin);
    // freopen("output.txt", "w", stdout);
    map<char, long long> letter_count;
    int size_of_grid, num_letters;
    cin >> size_of_grid >> num_letters;
    vector<vector<char>> grid(size_of_grid, vector<char>(size_of_grid, 0));
    vector<vector<long long>> grid_count(size_of_grid, vector<long long>(size_of_grid, 0));
    for (int row = 0; row < size_of_grid; row++)
    {
        int col = 0;
        string column;
        cin >> column;
        for (auto letter : column)
        {
            grid[row][col] = letter;
            col++;
        }
    }

    for (int row = 0; row < size_of_grid; row++)
    {
        for (int col = 0; col < size_of_grid; col++)
        {
            auto char_value = grid[row][col];
            long long left = (col - 1 < 0 || grid[row][col - 1] != char_value) ? 0 : grid_count[row][col - 1];
            long long top = (row - 1 < 0 || grid[row - 1][col] != char_value) ? 0 : grid_count[row - 1][col];
            long long diag = (row - 1 < 0 || col - 1 < 0 || grid[row - 1][col - 1] != char_value) ? 0 : grid_count[row - 1][col - 1];
            grid_count[row][col] = min(min(left, top), diag) + 1;
        }
    }
    // count the grids
    for (int row = 0; row < size_of_grid; row++)
    {
        for (int col = 0; col < size_of_grid; col++)
        {
            auto char_value = grid[row][col];
            auto count = grid_count[row][col];
            letter_count[char_value] += count;
        }
    }
    for (int i = 0; i < num_letters; i++)
    {
        auto value = 'A' + i;
        cout << letter_count[value] << endl;
    }
}