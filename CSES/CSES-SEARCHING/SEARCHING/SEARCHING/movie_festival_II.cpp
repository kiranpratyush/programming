#include <set>
#include <cstdio>
#include <vector>
#include <algorithm>
using namespace std;

typedef struct
{
    int start;
    int end;
} movie_time;

bool sort_by_endtime(movie_time &a, movie_time &b)
{
    if (a.end != b.end)
    {
        return a.end < b.end;
    }
    return a.start < b.start;
}

int main()
{
    // freopen("input.txt", "r", stdin);
    // freopen("output.txt", "w", stdout);
    vector<movie_time> times;
    multiset<int> end_times;
    int number_of_movies, club_members;
    scanf("%d %d", &number_of_movies, &club_members);
    for (int i = 0; i < club_members; i++)
    {
        end_times.insert(0);
    }
    for (int i = 0; i < number_of_movies; i++)
    {
        int start_time, end_time;
        scanf("%d %d", &start_time, &end_time);
        movie_time m = {start_time, end_time};
        times.push_back(m);
    }
    sort(times.begin(), times.end(), sort_by_endtime);
    int total_movies_can_be_watched = 0;
    for (auto x : times)
    {
        auto low_bound = end_times.upper_bound(x.start);
        if (low_bound != end_times.begin())
        {
            end_times.erase(--low_bound);
            end_times.insert(x.end);
            total_movies_can_be_watched += 1;
        }
    }
    printf("%d", total_movies_can_be_watched);
}