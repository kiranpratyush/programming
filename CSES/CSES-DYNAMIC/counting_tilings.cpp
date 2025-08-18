#include <cstdio>
#include <vector>
#define mod 1000000007
using namespace std;

void fill_colums(int index, int row_length, int mask, int new_mask, vector<int> &result_masks, bool is_last = false)
{
    if (index == row_length)
    {
        result_masks.push_back(new_mask);
        return;
    }
    // generic case : check if the index bit is set in the new_mask
    int is_set = (mask & (1 << index));
    if (is_set)
    {
        // unset the new_mask at the current index and move to new index (index+1)
        // How to unset a specific bit
        fill_colums(index + 1, row_length, mask, new_mask, result_masks, is_last);
    }
    else
    {
        // if bit is not set
        /*
            option1: start a horizontal block and make the new mask as one
            option2: if index<row_length-1 and index+1 is also not set then put a vertical block
            and unset the two indexes of the mask and go to index+2

        */
        int index_bit = (1 << index);
        if (!is_last)
        {
            fill_colums(index + 1, row_length, mask, (new_mask | index_bit), result_masks, is_last);
        }

        if (index < row_length - 1 && !(mask & (1 << (index + 1))))
        {
            fill_colums(index + 2, row_length, mask, new_mask, result_masks, is_last);
        }
    }
}

int count_tilings(int column, int max_row, int max_column, int mask, vector<vector<int>> &dp)
{
    if (column == max_column)
    {
        return 1;
    }
    if (dp[mask][column] != -1)
    {
        return dp[mask][column];
    }
    int total = 0;
    vector<int> result_mask;
    bool is_last = false;
    if (column == max_column - 1)
    {
        is_last = true;
    }
    fill_colums(0, max_row, mask, 0, result_mask, is_last);
    for (auto x : result_mask)
    {
        int val = count_tilings(column + 1, max_row, max_column, x, dp);
        total = (total + val) % mod;
    }
    dp[mask][column] = total;
    return total;
}

int main()
{
    // freopen("input.txt", "r", stdin);
    // freopen("output.txt", "w", stdout);
    int max_row, max_column;
    scanf("%d %d", &max_row, &max_column);
    vector<vector<int>> dp((1 << max_row) + 1, vector<int>(max_column + 1, -1));
    int result = count_tilings(0, max_row, max_column, 0, dp);
    printf("%d", result);
}