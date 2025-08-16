#include<cstdio>
#include<vector>
#include<algorithm>
using namespace std;

#define ll long long

typedef struct {
    ll start_time;
    ll end_time;
    ll weight;
} project;

bool comparator(project &a , project &b)
{
    if(a.end_time!=b.end_time)
    {
        return a.end_time<b.end_time;
    }
    return a.start_time<=b.start_time;
}

int main(){
    // freopen("input.txt","r",stdin);
    // freopen("output.txt","w",stdout);
    ll num_projects;
    scanf("%lld",&num_projects);
    vector<project>projects(num_projects);
    vector<ll>project_endtimes;
    vector<ll>dp(num_projects,0);

    for(int i=0;i<num_projects;i++)
    {   
        ll start_time,end_time,cost;
        scanf("%lld %lld %lld",&start_time,&end_time,&cost);
        projects[i] = {start_time,end_time,cost};
    }

    sort(projects.begin(),projects.end(),comparator);

    for(auto x:projects)
    {
        project_endtimes.push_back(x.end_time);
    }

    dp[0] = projects[0].weight;
    for(int i=1;i<num_projects;i++)
    {
        ll first_cost = projects[i].weight;
        ll position = lower_bound(project_endtimes.begin(), project_endtimes.begin() + i, projects[i].start_time) - project_endtimes.begin();
        position--; // Move to last project whose end_time <= start_time of current
        if(position >= 0)
        {
            first_cost += dp[position];
        }
        ll second_cost = dp[i-1];
        dp[i] = max(first_cost, second_cost);
    }

    printf("%lld\n", dp[num_projects-1]);
}
