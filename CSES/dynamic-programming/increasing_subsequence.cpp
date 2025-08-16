#include<cstdio>
#include<vector>
#include<algorithm>
#include<climits>

using namespace std;



int main(){

    // freopen("input.txt","r",stdin);
    // freopen("output.txt","w",stdout);
    int total_array_length;
    scanf("%d",&total_array_length);
    vector<int>array(total_array_length,0);
    for(int i=0;i<total_array_length;i++)
    {
        scanf("%d",&array[i]);
    }
    vector<int>longest_subsequence(total_array_length,INT_MAX);
    int max_length =0;
    for(int i=0;i<total_array_length;i++)
    {
        int element = array[i];
        int position = lower_bound(longest_subsequence.begin(),longest_subsequence.begin()+max_length,element)-longest_subsequence.begin();
        longest_subsequence[position] = element;
        if(position==max_length)
        {
            max_length = position+1;
        }
    }
    printf("%d",max_length);


}