import sys 
# sys.stdin = open("input.txt","r")
# sys.stdout = open("output.txt","w")



def palindrome_reorder(string):
    string_length = len(string)
    char_count = {}
    for char in string:
        if char not in char_count:
            char_count[char] =0
        char_count[char]+=1
    # count how many odd distinct char is present
    odd_char_count = 0
    odd_char = ""
    for char in char_count:
        if char_count[char]%2 !=0:
            odd_char_count+=1
            odd_char = char
    if(string_length%2==0):
        if(odd_char_count>0):
            return "NO SOLUTION"
        result = ["" for _ in range(string_length)]
        start_pointer = 0
        end_pointer = string_length-1
        for char in char_count:
            total_count = char_count[char]
            while(total_count>0):
                result[start_pointer]=char
                result[end_pointer]=char
                start_pointer+=1
                end_pointer-=1
                total_count-=2
        return "".join(result)
    else:
        if(odd_char_count>1):
            return "NO SOLUTION"
        result = ["" for _ in range(string_length)]
        odd_char = ""
        odd_char_count = 0
        start_pointer =0
        end_pointer =string_length-1
        for char in char_count:
            total_count = char_count[char]
            # first take the even count chars
            if(total_count%2!=0):
                odd_char = char
                odd_char_count = total_count
            else:
                while(total_count>0):
                    result[start_pointer]=char
                    result[end_pointer]=char
                    start_pointer+=1
                    end_pointer-=1
                    total_count-=2
        while(odd_char_count>0):
            result[start_pointer] = odd_char
            start_pointer+=1
            odd_char_count-=1
        return "".join(result)


if __name__=="__main__":
    string = input().strip()
    print(palindrome_reorder(string))

