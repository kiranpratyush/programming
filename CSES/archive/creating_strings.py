import sys
sys.setrecursionlimit(10**6)



def generate_permutations(character_array,character_count,pos,string_size,local_result_string,global_result_store):
    if pos ==string_size:
        global_result_store.append(local_result_string)
        return 1
    total_count=0
    for character in character_array:
        if character_count[character]>0:
            # that means the character have chance to be included
            character_count[character]-=1
            total_count+=generate_permutations(character_array,character_count,pos+1,string_size,local_result_string+character,global_result_store)
            # backtrack the count here automatically the local_result_string is backtracked
            character_count[character]+=1
    return total_count


if __name__=="__main__":
    character_string = input()
    character_counts = dict()
    for string in character_string:
        if string not in character_counts:
            character_counts[string] =0
        character_counts[string]+=1
    character_array = sorted(list(character_counts.keys()))
    global_result_store = []
    count = generate_permutations(character_array,character_counts,0,len(character_string),"",global_result_store)
    print(count)
    for result in global_result_store:
        print(result)
        