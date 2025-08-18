import sys
from queue import Queue

# sys.stdin = open("input.txt", "r")
# sys.stdout = open("output.txt", "w")


if __name__ == "__main__":
    num_courses, num_requirements = map(int, input().strip().split())
    graph = [[] for _ in range(num_courses)]
    indegree = [0 for _ in range(num_courses)]
    for _ in range(num_requirements):
        start_course, end_course = map(int, input().strip().split())
        start_course -= 1
        end_course -= 1
        graph[start_course].append(end_course)
        indegree[end_course] += 1
    queue = Queue()
    for course in range(num_courses):
        if indegree[course] == 0:
            queue.put(course)
    result = []
    length = 0
    while not queue.empty():
        course = queue.get()
        result.append(course)
        length += 1
        for dependent_course in graph[course]:
            indegree[dependent_course] -= 1
            if indegree[dependent_course] == 0:
                queue.put(dependent_course)
    if length == num_courses:
        result = [r + 1 for r in result]
        print(" ".join(map(str, result)))
    else:
        print("IMPOSSIBLE")
