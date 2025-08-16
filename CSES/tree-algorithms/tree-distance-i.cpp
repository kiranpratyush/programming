#include<cstdio>
#include<vector>

using namespace std;

typedef struct {
    int max_distance;
    int child;
} output;

void dfs(int current_node,int parent_node,vector<vector<int>>&tree,vector<output>&max_length,vector<output>&second_max_length)
{
    for(auto x:tree[current_node])
    {
        if(x!=parent_node)
        {
            dfs(x,current_node,tree,max_length,second_max_length);
            if(max_length[x].max_distance+1>max_length[current_node].max_distance)
            {
                second_max_length[current_node].max_distance = max_length[current_node].max_distance;
                second_max_length[current_node].child = max_length[current_node].child;
                max_length[current_node].max_distance = max_length[x].max_distance+1;
                max_length[current_node].child = x;
            }
            else if (max_length[x].max_distance+1>second_max_length[current_node].max_distance)
            {
                second_max_length[current_node].max_distance = max_length[x].max_distance+1;
                second_max_length[current_node].child = x;

            }
        }
        
    }

}
void dfs2(int current_node,int parent_node,vector<vector<int>>&tree,vector<output>&max_length,vector<output>&second_max_length)
{
    for(auto child:tree[current_node])
    {
        if(child!=parent_node)
        {
            // check if the max length of the parent does not go through the 
            if(max_length[current_node].child==child)
            {
                // check if the second max of the parent is more than the current child 
                if(second_max_length[current_node].max_distance+1>max_length[child].max_distance)
                {
                    second_max_length[child] = max_length[child];
                    max_length[child].max_distance = second_max_length[current_node].max_distance+1;
                    max_length[child].child = current_node;
                }
                else if(second_max_length[current_node].max_distance+1>second_max_length[child].max_distance){
                    second_max_length[child].max_distance = second_max_length[current_node].max_distance+1;
                    second_max_length[child].child = current_node;
                    
                }
            }
            else{
                second_max_length[child] = max_length[child];
                max_length[child].max_distance = max_length[current_node].max_distance+1;
                max_length[child].child = current_node;
            }
            dfs2(child,current_node,tree,max_length,second_max_length);
        }
       
    }
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
    vector<output>max_length(num_nodes);
    vector<output>second_max_length(num_nodes);
    for(int i=0;i<num_nodes;i++)
    {
        max_length[i].max_distance=0;
        max_length[i].child = -1;
        second_max_length[i].max_distance=0;
        second_max_length[i].child = -1;
    }
    dfs(0,-1,tree,max_length,second_max_length);
    dfs2(0,-1,tree,max_length,second_max_length);
    for (int i = 0; i < num_nodes; ++i) {
        printf("%d ", max_length[i].max_distance);
    }
}