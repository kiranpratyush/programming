/*
Prefix function definition

Longest proper prefix which is also the suffix of the string
Prefix function of string "aabaaab" is [0,1,0,1,2,2,3]
Reason : for substring till 1 : aa : Longest proper prefix is a and longest proper suffix is a
so the length is 1


*/

#include <vector>
#include <string>
#include <iostream>
using namespace std;
/*
The main idea is
if s[i] matches with the pi[i-1] then we can increase the length by 1
j = pi[j-1]
means next position where the same suffix matches as the j
*/
vector<int> prefix_function(string s)
{
    int n = (int)s.length();
    vector<int> pi(n);
    for (int i = 0; i < n; i++)
        for (int k = 0; k <= i; k++)
            if (s.substr(0, k) == s.substr(i - k + 1, k))
                pi[i] = k;
    return pi;
}
vector<int> prefix_function2(string s)
{
    int stringLength = s.size();
    vector<int> pi(stringLength);
    for (int i = 1; i < stringLength; i++)
    { /*
      To process index i find out the i-1 what is the element we can compare to
      if that matches then we can increment by one


      */
        int j = pi[i - 1];
        while (j > 0 && s[j] != s[i])
        {
            j = pi[j - 1];
        }
        if (s[i] == s[j])
        {
            j++;
        }
        pi[i] = j;
    }
    return pi;
}
int main()
{
    string s = "aabaaab";
    auto x = prefix_function(s);
    auto y = prefix_function2(s);
    for (auto t : x)
    {
        cout << t << endl;
    }
    for (auto t : y)
    {
        cout << t << endl;
    }
}