import sys

sys.setrecursionlimit(10**6)
# sys.stdin = open("input.txt", "r")
# sys.stdout = open("output.txt", "w")


def take_input():
    num_pupils, num_friendships = map(int, input().strip().split())
    graph = [[] for _ in range(num_pupils)]
    for _ in range(num_friendships):
        start_pupil, end_pupil = map(int, input().strip().split())
        # Make the pupils as 0 based indexing
        start_pupil -= 1
        end_pupil -= 1
        graph[start_pupil].append(end_pupil)
        graph[end_pupil].append(start_pupil)
    return graph, num_pupils, num_friendships


def dfs(graph, current_node, visited, teams, status):
    if not status[0]:
        return
    for neighbour in graph[current_node]:
        # if neighbour is not visited , this is a simple case
        # find out the team number of the current_node and assign the opposite
        if not visited[neighbour]:
            visited[neighbour] = True
            tobe_assigned_team = 2 if teams[current_node] == 1 else 1
            teams[neighbour] = tobe_assigned_team
            dfs(graph, neighbour, visited, teams, status)
        # if neigbour is visited , then check the team of the neigbour
        # if the current_node team is same as the neigbour mark the status as False
        # and return
        else:
            neighbour_assigned_team = teams[neighbour]
            current_node_assigned_team = teams[current_node]
            if neighbour_assigned_team == current_node_assigned_team:
                status[0] = False
                return
            else:
                continue


if __name__ == "__main__":
    graph, num_pupils, num_friendships = take_input()
    visited = [False for _ in range(num_pupils)]
    teams = [-1 for _ in range(num_pupils)]
    status = [True]
    for pupil in range(num_pupils):
        if not visited[pupil] and status[0]:
            teams[pupil] = 1
            visited[pupil] = True
            dfs(graph, pupil, visited, teams, status)
    if not status[0]:
        print("IMPOSSIBLE")
    else:
        print(" ".join(map(str, teams)))
