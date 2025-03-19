import sys


def find_num(position):
    start = 1
    base =9
    real_position = 0
    while(start*base<=position):
        position-=(start*base)
        real_position+=base
        start+=1
        base*=10
        
    # here process the remainder
    off_set_from_latest_base = position//start
    real_position+=off_set_from_latest_base
    remainder = position%start
    if remainder>=1:
        real_position+=1
        return str(real_position)[remainder-1]
    else:
        return str(real_position)[-1]


if __name__ == "__main__":
    total_queries = int(input())
    for _ in range(total_queries):
        position = int(input())
        x =find_num(position)
        print(x)
            
                
    