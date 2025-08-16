#include<vector>
#include<cstdio>
#include<climits>
using namespace std;


vector<int>get_digits(int n)
{
    vector<int>digits;
    while(n>0)
    {
        int digit = n%10;
        n = n/10;
        digits.push_back(digit);
    }
    return digits;

}


int main(){

// freopen("input.txt","r",stdin);
// freopen("output.txt","w",stdout);
int input_number;
scanf("%d",&input_number);
vector<int>dp(input_number+1,INT_MAX);
dp[0] = 0;
for(int i=1;i<=input_number;i++)
{
    auto digits = get_digits(i);
    int min_count = INT_MAX;
    for(auto digit:digits)
    {
        int new_number = i-digit;
        if(dp[new_number]!=INT_MAX)
        {
            min_count = min(min_count,dp[new_number]+1);
        }
    }
    dp[i] = min_count;
}
printf("%d",dp[input_number]);



}