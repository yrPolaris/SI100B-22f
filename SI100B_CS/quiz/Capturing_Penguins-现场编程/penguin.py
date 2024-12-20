# ============= YOU NEED TO WRITE YOUR CODE IN THE FUNCTION =============
def solve(num_rows, num_columns, actions):
    lst = []
    for i in range(num_columns):
        lst.append([])
        for i in range(num_rows):
            lst[-1].append([])
    for i in actions:
        direction = i[0]
        row = i[1]
        num = i[2]
        count = num_columns
        if direction == "right":
            while num >= 1 and count >= 1:
                num = num - 1
                count = count - 1 
                if lst[count][row] == []:
                    lst[count].pop(row)
                    lst[count].insert(num_rows - 1,1)
                else:
                    num = num + 1
                    continue
        if direction == "left":
            while num >= 1 and count >= 1:
                num = num - 1
                count = count - 1 
                if lst[-count - 1][row] == []:
                    lst[-count-1].pop(row)
                    lst[-count-1].insert(num_rows - 1,1)
                else:
                    num = num + 1
                    continue
    ans = []
    for i in range(num_columns):
        ans.append([])
    count_row = 0
    count_column = 0
    for i in range(num_columns):
        count_column += 1
        number1 = 0
        count_row = 0
        for i in range(num_rows):
            count_row += 1
            if lst[count_column - 1][count_row - 1] == []:
                number1 += 1
        ans[count_column - 1] = number1
    return ans


# ========== DON'T MODIFY THIS ==========
def main():
    num_rows, num_columns = map(int, input().split()) # Input the number of columns and the number of rows
    actions = eval(input()) # Input the list of actions
    print(solve(num_rows, num_columns, actions))

main()
