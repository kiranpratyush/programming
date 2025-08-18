import sys
import heapq as hq
import math

# sys.stdin = open("input.txt", "r")
# sys.stdout = open("output.txt", "w")


def take_input():
    num_cities, num_roads = map(int, input().strip().split())
    graph = [[] for _ in range(num_cities)]
    for _ in range(num_roads):
        start_road, end_road, weight = map(int, input().strip().split())
        # make the road as the 0 based indexing
        start_road -= 1
        end_road -= 1
        graph[start_road].append((end_road, weight))
    return graph, num_cities, num_roads


def dijkstra(graph, num_cities, num_roads):
    queue = [(0, 0)]
    distance = [math.inf for _ in range(num_cities)]
    distance[0] = 0
    length = 1
    while length > 0:
        city_distance, city = hq.heappop(queue)
        length -= 1
        if city_distance > distance[city]:
            continue
        for neigbour in graph[city]:
            end_city, weight = neigbour
            if distance[end_city] > weight + city_distance:
                hq.heappush(queue, (weight + city_distance, end_city))
                distance[end_city] = weight + city_distance
                length += 1
    return distance


if __name__ == "__main__":
    graph, num_cities, num_roads = take_input()
    distances = dijkstra(graph, num_cities, num_roads)
    print(" ".join(map(str, distances)))
