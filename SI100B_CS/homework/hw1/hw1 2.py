row, column = map(int,input().split())
final = row * column
count = 0
top_row = 0
top_column = 0
ls = []
row_coordinate, column_coordinate = 0, 0
for i in range(row):
    ls.append([])
    for j in range(column):
        ls[-1].append([])
while count != final:
    if row_coordinate == top_row and column_coordinate < column:
        count += 1
        ls[row_coordinate][column_coordinate] = count
        column_coordinate += 1
        if column_coordinate == column:
            column -= 1
            column_coordinate -= 1
    elif row_coordinate < row - 1 and column_coordinate == column:
        row_coordinate += 1
        count += 1
        ls[row_coordinate][column_coordinate] = count
        if row_coordinate == row - 1:
            row -= 1
    elif row_coordinate == row and column_coordinate > top_column:
        column_coordinate -= 1
        count += 1
        ls[row_coordinate][column_coordinate] = count
        if column_coordinate == top_column:
            top_row += 1
    elif row >= row_coordinate > top_row and column_coordinate == top_column:
        row_coordinate -= 1
        count += 1
        ls[row_coordinate][column_coordinate] = count
        if row_coordinate == top_row:
            column_coordinate += 1
            top_column += 1
    else:
        continue
print(ls)

