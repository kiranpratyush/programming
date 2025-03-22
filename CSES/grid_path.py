import sys
sys.setrecursionlimit(10**6)
sys.stdin = open("input.txt","r")
sys.stdout = open("output.txt","w")

move_mapper = {"L":(0,-1),"R":(0,1),"U":(-1,0),"D":(1,0)}



def grid_path(visited,input_string,input_string_length,current_index,curr_posx,curr_posy):
        
    if current_index==input_string_length:
        if curr_posx==6 and curr_posy==0:
            return 1
        return 0
    if curr_posx==6 and curr_posy==0:
        return 0
    if curr_posx ==0 or curr_posx==6:
        if curr_posy>0 and curr_posy<6 and not visited[curr_posx][curr_posy-1] and not visited[curr_posx][curr_posy+1]:
            return 0
    if curr_posy==0 or curr_posy==6:
        if curr_posx >0 and curr_posx<6 and not visited[curr_posx-1][curr_posy] and not visited[curr_posx+1][curr_posy]:
            return 0
    if curr_posx>0 and curr_posx <6 and curr_posy>0 and curr_posy<6:
        if visited[curr_posx-1][curr_posy] and visited[curr_posx+1][curr_posy] and not visited[curr_posx][curr_posy+1] and not visited[curr_posx][curr_posy-1]:
            return 0
        if visited[curr_posx][curr_posy-1] and visited[curr_posx][curr_posy+1] and not visited[curr_posx-1][curr_posy] and not visited[curr_posx+1][curr_posy]:
            return 0
   
    
    next_move = input_string[current_index]
    available_moves = ["L","R","U","D"]
    if next_move!="?":
        available_moves = [next_move]
    feasible_move=""
    if curr_posx==1 and curr_posy==0 and not visited[0][0]:
        feasible_move = "U"
    elif curr_posx==0 and curr_posy==1 and not visited[0][0]:
        feasible_move = "L"
    elif curr_posx==0 and curr_posy==5 and not visited[0][6]:
        feasible_move = "R"
    elif curr_posx==1 and curr_posy==6 and not visited[0][6]:
        feasible_move ="U"
    elif curr_posx==5 and curr_posy==6 and not visited[6][6]:
        feasible_move = "D"
    elif curr_posx ==6 and curr_posy==5 and not visited[6][6]:
        feasible_move="R"
    
    if feasible_move!="":
        curr_move = []
        for move in available_moves:
            if move==feasible_move:
                curr_move.append(move)
        available_moves = curr_move
    
    count = 0
    for move in available_moves:
        move_x,move_y = move_mapper[move]
        next_move = (curr_posx+move_x,curr_posy+move_y)
        if next_move[0]>=0 and next_move[0]<=6 and next_move[1]>=0 and next_move[1]<=6 and not visited[next_move[0]][next_move[1]]:
            visited[next_move[0]][next_move[1]] = True
            count+= grid_path(visited,input_string,input_string_length,current_index+1,next_move[0],next_move[1])
            visited[next_move[0]][next_move[1]] = False
    return count




if __name__ =='__main__':
    input_string = input()
    visited = [[False for _ in range(7)] for _ in range(7)]
    visited[0][0] = True
    if input_string[-1] !="?" and not (input_string[-1]!="L" or input_string[-1]!="D"):
        print("Rinning", input_string[-1]!="D")
        print(0)
    elif input_string[-2] !="?" and not ( input_string[-2]!="L" or input_string[-2]!="D"):
        print(0)
    else:
        print(grid_path(visited,input_string,len(input_string),0,0,0))