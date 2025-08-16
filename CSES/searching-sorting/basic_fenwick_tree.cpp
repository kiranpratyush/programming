#include<cstdio>
#include<vector>
using namespace std;

int read(vector<int>&tree,int index){
    int value =0;
    while(index>0){
        value+=tree[index];
        index-=(index&-index);
    }
    return value;

}

void update(vector<int>&tree,int index,int maxIndex,int value){
    while(index<=maxIndex){
        tree[index] +=value;
        index+=(index&-index);
    }

}



int main(){
freopen("input.txt","r",stdin);
freopen("output.txt","w",stdout);
vector<int>v;
v.push_back(2);
v.push_back(4);
v.push_back(5);
v.push_back(7);
v.push_back(3);
vector<int>tree(8,0);
int max_range = 7;
for(int i=0;i<5;i++){
    int index = v[i];
    update(tree,index,max_range,1);
}
// How many numbers are less than or equal to 3 answer 2 
int result;
result = read(tree,7);
printf("%d",result);


return 0;



}