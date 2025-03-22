import sys
sys.setrecursionlimit(10**6)
# sys.stdin = open("input.txt","r")
# sys.stdout = open("output.txt","w")


def gray_code(n):
    if n==1:
        return ["0","1"]
    smaller_gray = gray_code(n-1)
    gray_1 = ["0"+x for x in smaller_gray]
    gray_2 = ["1"+x for x in smaller_gray]
    result_gray = gray_1+gray_2[::-1]
    return result_gray


if __name__ == "__main__":
    number = int(input())
    result = gray_code(number)
    for r in result:
        print(r)