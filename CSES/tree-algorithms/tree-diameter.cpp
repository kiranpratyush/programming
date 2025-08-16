#include<cstdio>
#include<vector>
using namespace std;


typedef struct {
    int diameter;
    int max_length;
} output;

output dfs(int current_node,int parent_node,vector<vector<int>>&tree)
{
    int first_max_length =0;
    int second_max_length=0;
    int max_diameter = 0;
    for(auto child:tree[current_node])
    {
        if(child!=parent_node)
        {
            auto output = dfs(child,current_node,tree);
            if(output.max_length>=first_max_length)
            {
                second_max_length = first_max_length;
                first_max_length = output.max_length;
                
            }
            else if(output.max_length>second_max_length)
            {
                second_max_length = output.max_length;
            }
            max_diameter = max(max_diameter,output.diameter);
        }
    }
    // create new diameter here
    max_diameter = max(max_diameter,first_max_length+second_max_length+1);
    int new_max_length = first_max_length+1;
    return {max_diameter,new_max_length};
}

int main()
{
    // freopen("input.txt","r",stdin);
    // freopen("output.txt","w",stdout);
    int num_nodes,num_edges;
    scanf("%d",&num_nodes);
    num_edges = num_nodes-1;
    vector<vector<int>>tree(num_nodes);
    while(num_edges>0)
    {
        int start_node,end_node;
        scanf("%d %d",&start_node,&end_node);
        tree[start_node-1].push_back(end_node-1);
        tree[end_node-1].push_back(start_node-1);
        num_edges--;
    }
    auto x = dfs(0,-1,tree);
    printf("%d",x.diameter-1);
}