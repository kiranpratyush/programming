# This is a problem of finding the simple cycle
import sys

sys.setrecursionlimit(10**6)
# sys.stdin = open("input.txt", "r")
# sys.stdout = open("output.txt", "w")


## Just building the graph
def take_input():
    num_cities, num_roads = map(int, input().strip().split())
    graph = [[] for _ in range(num_cities)]
    for _ in range(num_roads):
        start_road, end_road = map(int, input().strip().split())
        # make the road 0 based indexing
        start_road -= 1
        end_road -= 1
        graph[start_road].append(end_road)
        graph[end_road].append(start_road)
    return graph, num_cities, num_roads


def dfs(graph, visited, current_node, parent_mapper, status, paths):
    # Here if the cycle is found earlier then just return
    if status[0]:
        return
    for neighbour in graph[current_node]:
        # Here if the cycle is found earlier then just return
        if status[0]:
            return
        # This is a simple case if the neighbour is not visited then
        # jsut do the dfs with keeping the parent mapper
        if not visited[neighbour]:
            visited[neighbour] = True
            parent_mapper[neighbour] = current_node
            dfs(graph, visited, neighbour, parent_mapper, status, paths)
        else:
            # If the neigbour is visited and it is not the parent then we have encountered a cycle
            if neighbour != parent_mapper[current_node]:
                # Here we have found the cycle
                # first append the end of the cycle
                paths.append(neighbour)
                result_node = current_node
                while result_node != neighbour:
                    # Here traverse back the cycle
                    paths.append(result_node)
                    result_node = parent_mapper[result_node]
                # Finally append the neigbour
                # Remember this is now the reversed of how the paths are visited
                # As we are traversing backwards
                paths.append(neighbour)
                # Mark the status as found , this will cause early return
                status[0] = True
                return


if __name__ == "__main__":
    graph, num_cities, num_roads = take_input()
    paths = []
    status = [False]
    parent_mapper = [node for node in range(num_cities)]
    visited = [False for _ in range(num_cities)]
    # This is required if the graph is disjoint
    for city in range(num_cities):
        if not visited[city]:
            visited[city] = True
            dfs(graph, visited, city, parent_mapper, status, paths)
            # if we have found the cycle then just reverse the path
            # Make the paths one based indexing
            # Print the path
            if status[0]:
                paths = paths[::-1]
                # make it 1 based index
                paths = list(map(lambda x: x + 1, paths))
                print(len(paths))
                print(" ".join(map(str, paths)))
                break
    # If we never found the paths then just print IMPOSSIBLE
    if not status[0]:
        print("IMPOSSIBLE")
