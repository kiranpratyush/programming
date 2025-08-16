import sys

# Read input and output from files
# sys.stdin = open("../input.txt", "r")
# sys.stdout = open("../output.txt", "w")


def is_valid(board, posx, posy):
    """
    Check if a queen can be placed at (posx, posy) without being attacked.
    """
    # Ensure the position is within bounds and empty
    if not (0 <= posx < 8 and 0 <= posy < 8 and board[posx][posy] == '.'):
        return False

    # Check previous rows only (since we are placing queens row-wise)
    for row in range(posx):
        for col in range(8):
            if board[row][col] == 'x':
                # Same column
                if col == posy:
                    return False
                # Same diagonal
                if abs(row - posx) == abs(col - posy):
                    return False
    return True


def calculate_count_of_possibility(board, piece_no):
    """
    Recursively place queens and count valid configurations.
    """
    if piece_no == 8:  # Stop at 8 queens, not 9
        return 1  # Found one valid way to place all queens

    total = 0
    for col in range(8):
        if is_valid(board, piece_no, col):  # Try placing the queen in this column
            board[piece_no][col] = 'x'
            total += calculate_count_of_possibility(board, piece_no + 1)
            board[piece_no][col] = '.'  # Backtrack

    return total


if __name__ == "__main__":
    # Read the board input
    board = [list(input().strip()) for _ in range(8)]  

    # Start placing queens from row 0
    result = calculate_count_of_possibility(board, 0)

    # Print the final count of valid queen placements
    print(result)
