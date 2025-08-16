import sys
# sys.stdin = open("input.txt","r")
# sys.stdout = open("output.txt","w")



def calculate_apple_weights(weights,pos,group1,group2,length):
    if(pos==length):
        return abs(group1-group2)
    # You have two choice now include the item in the group1 or group2
    result1 = calculate_apple_weights(weights,pos+1,group1+weights[pos],group2,length)
    result2 = calculate_apple_weights(weights,pos+1,group1,group2+weights[pos],length)
    return min(result1,result2)
    

if __name__=="__main__":
    total_apples = int(input())
    apple_weights = list(map(int,input().split()))
    result = calculate_apple_weights(apple_weights,0,0,0,total_apples)
    print(result)