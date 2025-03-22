import sys


def toi(n,start,middle,end,result):
    if(n==1):
        result.append((start,end))
        return 1
    first_move = toi(n-1,start,end,middle,result)
    result.append((start,end))
    second_move = toi(n-1,middle,start,end,result)
    return first_move+second_move+1


if __name__=="__main__":
    x = int(input())
    result = []
    total = toi(x,1,2,3,result)
    print(total)
    for i in result:
        print(*i)
