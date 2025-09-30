/*
Manachar's algorithm
- Find all the palindromic substring in a string
aabac -> aba is a palindromic substring
aabac Brute force approach
for each string find all the substring
check if it is palindrome  or not



*/
#include <algorithm>
#include <string>
#include <iostream>

void palindromicSubstringInBruteForce(std::string s)
{
    for (int i = 0; i < s.size(); i++)
    {
        for (int j = i; j < s.size(); j++)
        {
            std::string s2 = s.substr(i, j - i + 1);
            std::string s3 = s2;
            std::reverse(s3.begin(), s3.end());
            if (s2 == s3)
            {
                std::cout << s2 << std::endl;
            }
        }
    }
}
void AugmentString(std::string s)
{
    std::string newString = "#";
    for (auto x : s)
    {
        newString += x;
        newString += "#";
    }
    std::cout << newString;
}

int main()
{
    std::string s = "abc";
    palindromicSubstringInBruteForce(s);
    AugmentString(s);
}