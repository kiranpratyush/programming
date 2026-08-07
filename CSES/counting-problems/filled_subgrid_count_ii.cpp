#include <bits/stdc++.h>
using namespace std;

typedef struct
{
    long long index;
    long long width;
    long long count;
} stack_entry;

int main()
{
    // freopen("input.txt", "r", stdin);
    // freopen("output.txt", "w", stdout);

    map<char, long long> letter_count;
    int size_of_grid, num_letters;
    cin >> size_of_grid >> num_letters;

    vector<vector<char>> grid(
        size_of_grid,
        vector<char>(size_of_grid, 0));

    vector<pair<char, int>> vertical_width(
        size_of_grid,
        {'A', 0});

    vector<stack_entry> stack{};
    map<char, long long> rectangle_count{};

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
        stack = {};

        for (int col = 0; col < size_of_grid; col++)
        {
            int vertical_length = 1;
            int starting_span = col;
            auto current_letter = grid[row][col];

            if (row > 0 &&
                vertical_width[col].first == current_letter)
            {
                vertical_length =
                    vertical_width[col].second + 1;
            }

            vertical_width[col] =
                {current_letter, vertical_length};

            long long count = 0;

            if (col == 0 ||
                grid[row][col - 1] != current_letter)
            {
                stack = {};
            }

            while (!stack.empty() &&
                   stack.back().width > vertical_length)
            {
                starting_span = stack.back().index;
                stack.pop_back();
            }

            if (stack.empty())
            {
                count =
                    (col - starting_span + 1LL) *
                    vertical_length;

                stack.push_back(
                    {starting_span, vertical_length, count});
            }
            else if (stack.back().width < vertical_length)
            {
                count =
                    stack.back().count +
                    (col - starting_span + 1LL) *
                        vertical_length;

                stack.push_back(
                    {starting_span, vertical_length, count});
            }
            else
            {
                count =
                    stack.back().count +
                    (col - starting_span + 1LL) *
                        vertical_length;

                stack.back().count = count;
            }

            rectangle_count[current_letter] += count;
        }
    }
    for (int i = 0; i < num_letters; i++)
    {
        auto letter = 'A' + i;
        cout << rectangle_count[letter] << endl;
    }
}