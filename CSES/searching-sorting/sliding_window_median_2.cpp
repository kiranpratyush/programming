/*
1. Take the input and build the compress and the decompress mapping 
2. build the add function 
3. build the update function
4. get the result
*/

#include<map>
#include<vector>
#include<cstdio>
using namespace std;
#define MAX_SIZE 200005

void add(vector<int>&result,int index)
{
    while(index<=MAX_SIZE)
    {
        result[index]+=1;
        index+=(index&-index);
    }
}
void remove(vector<int>&result,int index){
    while(index<=MAX_SIZE){
        result[index]-=1;
        index+=(index&-index);
    }
}

int read(vector<int>&tree,int index)
{
    int result =0;
    while(index!=0)
    {
        result+=tree[index];
        index-=(index&-index);

    }
    return result;
}



int main(){
// freopen("../input.txt","r",stdin);
// freopen("../output.txt","w",stdout);
int number_element,window_size;
scanf("%d %d",&number_element,&window_size);

map<int,int>compress;
map<int,int>decompress;
vector<int>array(number_element+1,0);
vector<int>tree(MAX_SIZE+1,0);
vector<int>result;
// get the input 
for(int i=1;i<=number_element;i++){
    int element;
    scanf("%d",&element);
    array[i] = element;
    compress[element] =0;
}
// Now compress the element 
int index =1;
for(auto &x:compress)
{
    x.second = index;
    decompress[x.second] = x.first;
    index++;
}
for(int i=1;i<=number_element;i++)
{   
    int index = compress[array[i]];
    // update the tree
    add(tree,index);
    if(i>=window_size)
    {
        // get the result here and remove the first element here 
        int cummulative_frequency = (window_size/2)+(window_size&1);
        int low =1;
        int high = MAX_SIZE;
        while(low<=high)
        {
            int mid =(low+high)/2;
            int curr_value = read(tree,mid);
            int prev_value = read(tree,mid-1);
            if(curr_value>=cummulative_frequency && prev_value<cummulative_frequency)
            {
                result.push_back(decompress[mid]);
                break;
            }
            else if(curr_value<cummulative_frequency)
            {
                low = mid+1;
            }
            else{
                high = mid-1;
            }
        }
        

        remove(tree,compress[array[i-window_size+1]]);
    }
}
for(auto x:result)
{
    printf("%d ",x);
}

}

