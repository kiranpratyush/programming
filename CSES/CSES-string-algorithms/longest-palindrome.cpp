#include <iostream>
#include <string>
#include <vector>
using namespace std;

int main()
{
    // freopen("input.txt", "r", stdin);
    // freopen("output.txt", "w", stdout);
    string inputString;
    cin >> inputString;
    // Build the new string augmented with @#
    string augMentedString = "@";
    for (auto c : inputString)
    {
        augMentedString += "#";
        augMentedString += c;
    }
    augMentedString += "#$";
    int center = 0;
    int rightIndex = 0;
    vector<int> P(augMentedString.size() + 1, 0);
    for (int i = 1; i < augMentedString.size() - 1; i++)
    {
        int iMirror = center - (i - center);
        if (rightIndex > i)
        {
            P[i] = min(P[iMirror], rightIndex - i);
        }
        while (augMentedString[i + 1 + P[i]] == augMentedString[i - 1 - P[i]])
        {
            P[i]++;
        }
        if (i + P[i] > rightIndex)
        {
            rightIndex = i + P[i];
            center = i;
        }
    }
    // build the max palindrome
    int maxPalindromSize = 0;
    int index = 0;
    for (int i = 1; i < augMentedString.size() - 1; i++)
    {

        if (P[i] > maxPalindromSize)
        {
            maxPalindromSize = P[i];
            index = i;
        }
    }
    cout << inputString.substr((index - 1 - maxPalindromSize) / 2, maxPalindromSize) << endl;
}