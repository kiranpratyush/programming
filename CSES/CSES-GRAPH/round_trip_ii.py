import sys

sys.setrecursionlimit(10**6)
# sys.stdin = open("input.txt", "r")
# sys.stdout = open("output.txt", "w")


def dfs(graph, city, local_visited, global_visited, result):
    local_visited[city] = True
    global_visited[city] = True
    for dest_city in graph[city]:
        if not global_visited[dest_city]:
            status, terminated, result_node = dfs(
                graph, dest_city, local_visited, global_visited, result
            )
            if status:
                if not terminated:
                    result.append(dest_city)
                    if result_node == dest_city:
                        terminated = True
                return status, terminated, result_node
        elif local_visited[dest_city]:
            ## That means both the global visited and the local visited is true
            result.append(dest_city)
            return True, False, dest_city
    result = []
    local_visited[city] = False
    return False, False, city


if __name__ == "__main__":
    num_cities, num_flights = map(int, input().strip().split())
    graph = [[] for _ in range(num_cities)]
    for _ in range(num_flights):
        start_city, end_city = map(int, input().strip().split())
        # Make the city 0 based indexing
        start_city -= 1
        end_city -= 1
        # Build the graph (directed graph as the problem tells that the connection is oneway)
        graph[start_city].append(end_city)
    global_visited = [False for _ in range(num_cities)]
    local_visited = [False for _ in range(num_cities)]
    found = False
    result = []
    for city in range(num_cities):
        if not global_visited[city]:
            status, _, _ = dfs(graph, city, local_visited, global_visited, result)
            if status:
                found = True
                if result[0] != result[-1]:
                    result.append(result[0])
                new_result = []
                for _ in result[::-1]:
                    new_result.append(_ + 1)
                print(len(new_result))
                print(" ".join(map(str, new_result)))

                break
    if not found:
        print("IMPOSSIBLE")
