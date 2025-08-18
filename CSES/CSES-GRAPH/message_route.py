from queue import Queue

# import sys

# sys.stdin = open("input.txt", "r")
# sys.stdout = open("output.txt", "w")


def take_input():
    num_computers, num_connections = map(int, input().strip().split())
    graphs = [[] for _ in range(num_computers)]
    for _ in range(num_connections):
        # convert this as a 0 based indexing
        start_computer, end_computer = map(int, input().strip().split())
        start_computer -= 1
        end_computer -= 1
        graphs[start_computer].append(end_computer)
        graphs[end_computer].append(start_computer)
    return graphs, num_computers, num_connections


def bfs():
    graphs, num_computers, num_connections = take_input()
    queue = Queue()
    visited = [False for _ in range(num_computers)]
    queue.put(0)
    visited[0] = True
    parent_mapper = [-1 for _ in range(num_computers)]
    parent_mapper[0] = 0
    while not queue.empty():
        computer = queue.get()
        if computer == num_computers - 1:
            length = 0
            paths = []
            end_computer = computer
            while parent_mapper[end_computer] != end_computer:
                paths.append(end_computer)
                end_computer = parent_mapper[end_computer]
                length += 1
            paths.append(0)
            length += 1
            return True, length, paths

        for neighbour in graphs[computer]:
            if not visited[neighbour]:
                queue.put(neighbour)
                visited[neighbour] = True
                parent_mapper[neighbour] = computer
    return False, 0, []


if __name__ == "__main__":
    status, length, paths = bfs()
    # make the paths 1 based indexing
    paths = list(map(lambda x: x + 1, paths))
    paths = paths[::-1]
    if status:
        print(length)
        print(" ".join(map(str, paths)))
    else:
        print("IMPOSSIBLE")
