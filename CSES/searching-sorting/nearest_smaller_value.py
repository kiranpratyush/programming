# import sys
# sys.stdin = open("../input.txt","r")
# sys.stdout = open("../output.txt","w")



if __name__=="__main__":
    result = []
    n = int(input())
    stack = []
    length =0
    elements = list(map(int,input().split()))
    for index,element in enumerate(elements):
        while(length>0 and stack[-1][1]>=element):
            stack.pop()
            length-=1
        if(length>0):
            result.append(stack[-1][0]+1)
        else:
            result.append(0)
        stack.append((index,element))
        length+=1
    print(" ".join(map(str,result)))