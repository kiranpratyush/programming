#include<cstdio>
#include<set>
#include<vector>
using namespace std;

#define ll long long


int main(){

    // freopen("../input.txt","r",stdin);
    // freopen("../output.txt","w",stdout);
    ll num_elements,window_size;
    vector<ll>array;
    scanf("%lld %lld",&num_elements,&window_size);
    for(int i=0;i<num_elements;i++)
    {
        ll element;
        scanf("%lld",&element);
        array.push_back(element);
    }
    if(window_size==1)
    {
        for(int i=0;i<num_elements;i++)
        {
            printf("%d ",0);
        }
        return 0;
    }

    multiset<ll>low_set;
    multiset<ll>high_set;
    ll low_allowed_size = (window_size/2)+(window_size&1);
    ll high_allowed_size = (window_size/2);
    ll low_sum =0;
    ll high_sum=0;
    vector<ll>result;
    // push the first element in the low_set
    low_set.insert(array[0]); // considering the number of elements should be >1
    low_sum = array[0];

    for(int i=1;i<num_elements;i++)
    {   // now find the median and calculate the cose
       
        ll element = array[i];

        if(i>=window_size)
        {
            // first remove the element ;
            ll element_to_be_removed = array[i-window_size];
            // first remove the elment from the high_sum if exists
            if(high_set.find(element_to_be_removed)!=high_set.end())
            {
                high_set.erase(high_set.find(element_to_be_removed));
                high_sum-=element_to_be_removed;
            }
            else{
                // remove from the low set if the low set become empty remove the smaller
                // number from the high set and push it to the low set
                low_set.erase(low_set.find(element_to_be_removed));
                low_sum-=element_to_be_removed;
                if(low_set.size()==0)
                {
                    ll high_element = *high_set.begin();
                    high_set.erase(high_set.find(*high_set.rbegin()));
                    high_sum-=high_element;
                    low_set.insert(high_element);
                    low_sum+=high_element;
                }
            }
        }

        
        // check if the element is <= to the last element of the low_set
        if(!low_set.empty()&&element<=(*low_set.rbegin()))
        {
            // first insert the element;
            low_set.insert(element);
            low_sum+=element;
            // now check if the size of the low_set is larger than the allowed size
            if(low_set.size()>low_allowed_size)
            {
                // remove the largest one and add it to the high_set
                ll element_to_be_removed = *low_set.rbegin();
                low_set.erase(--low_set.end());
                low_sum-=element_to_be_removed;
                high_set.insert(element_to_be_removed);
                high_sum+=element_to_be_removed;
            }
        }
        else{
            // add to the high_set
            high_set.insert(element);
            high_sum+=element;
            if(high_set.size()>high_allowed_size)
            {   // remove the first element and add it to the low_set
                ll element_to_be_removed = *high_set.begin();
                high_set.erase(high_set.begin());
                high_sum-=element_to_be_removed;
                low_set.insert(element_to_be_removed);
                low_sum+=element_to_be_removed;
            }
        }
        if(i>=window_size-1)
        {
            ll median = *low_set.rbegin();
            ll total_low_sum = median*(low_set.size());
            ll total_high_sum = median*(high_set.size());
            ll cost = (total_low_sum-low_sum)+(high_sum-total_high_sum);
            result.push_back(cost);
        }

        


    }
    
    for(auto x:result){
        printf("%lld ",x);
    }
}
