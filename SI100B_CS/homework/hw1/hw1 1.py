# This program was designed to compare the medians of two lists, written by the soldier Gary Zhang.
# However, after he finished the code, he found there were so many bugs!
# Your goal is to help him make the poor code act as wished, or at least... make the code run!
# There are a few things to notice:
    # 1. The lengths of the two lists are always equal and is >= 1.
    # 2, When the length is odd, the larger of the two numbers in the middle of the lists will be given to Imperial army.
    # 3. There exist both syntax mistakes and logical mistakes, so be careful.

# The code below deals with the inputs.
# ========== DON'T MODIFY THIS ==========
i_list = list(map(int, input().split())) # The list of Imperial army 
e_list = list(map(int, input().split())) # The list of Enemy army
# =======================================

# ============= YOU NEED TO MODIFY THE CODE BELOW =============

# n is the length of the two lists.
n = len(i_list)

# At first, Gary made a loop to swap the numbers in the lists,
# but there seems to be some problems.
for i in range(0, n):
    if i < n/2:
        if i_list[i] < e_list[i]:
            i_list[i], e_list[i] = e_list[i], i_list[i]
    if i > n/2:
        if i_list[i] > e_list[i]:
            i_list[i], e_list[i] = e_list[i], i_list[i]
# Sort the two lists.
i_list.sort()
e_list.sort()

# After sorting the list, Gary began to calculate their medians.
if n % 2 == 1:
    i_median = i_list[int((n-1) / 2)]
    e_median = e_list[int((n-1) / 2)]
else:
    i_median = ( (i_list[int((n / 2)) - 1] + i_list[int((n / 2))]) ) / 2
    e_median = ( (e_list[int((n / 2)) - 1] + e_list[int((n / 2))]) ) / 2

# Finally, it's time to compare their medians.
if i_median > e_median:
    print("Imperial army has larger median!")
elif i_median < e_median:
    print("Enemy army has larger median!")
else:
    print("The two medians are equal!")
