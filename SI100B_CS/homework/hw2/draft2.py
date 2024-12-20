# ========================================TASK2====================================================
def Act2():
#====================================YOUR CODE HERE================================================
    rd_num, hp = input().split(",")
    rd_num = eval(rd_num.replace(" ",""))
    hp = eval(hp.replace(" ",""))
    hd_cards = []
    hd_cards10 = input().replace(" ","").split(",")
    for i in hd_cards10:
        if i != "" and i != " ":
            hd_cards.append(i)
    final_ans = []
    ls2 = hd_cards
    ls3 = []
    ls4 = []
    ls5 = []
    ls6 = []
    ans = []
    dict1 = dict()
    dict2 = dict()
    for i in ls2:
        if i[1] == "A":
             dict1["z"] = i
        if i[1] == "2":
             dict1["a"] = i
        if i[1] == "3":
            dict1["b"] = i
        if i[1] == "4":
            dict1["c"] = i
        if i[1] == "5":
            dict1["d"] = i
        if i[1] == "6":
            dict1["e"] = i
        if i[1] == "7":
            dict1["f"] = i
        if i[1] == "8":
            dict1["g"] = i
        if i[1] == "9":
            dict1["h"] = i
        if i[1] == "T":
            dict1["i"] = i
        if i[1] == "J":
            dict1["j"] = i
        if i[1] == "Q":
            dict1["k"] = i
        if i[1] == "K":
            dict1["l"] = i
        x1 = list(dict1.popitem())
        ls3.append(x1)
    ls3.sort(reverse = True ,key = lambda x:x[0])
    for i in ls3:
        ls4.append(i[1])
    ls4.append("12")
    x = 0
    for i in range(len(ls4)-1):
        x += 1
        if ls4[x-1][1] == ls4[x][1]:
            ls5.append(ls4[x-1])
            if x == len(ls4)-1:
                for i in ls5:
                    if i[0] == "D":
                        dict2["a"] = i
                    if i[0] == "C":
                        dict2["b"] = i
                    if i[0] == "H":
                        dict2["c"] = i
                    if i[0] == "S":
                        dict2["d"] = i
                    x2 = list(dict2.popitem())
                    ls6.append(x2)
                ls6.sort(reverse = True ,key = lambda x:x[0])
                for i in ls6:
                    ans.append(i[1])
        if ls4[x-1][1] != ls4[x][1]:
            ls5.append(ls4[x-1])
            for i in ls5:
                if i[0] == "D":
                    dict2["a"] = i
                if i[0] == "C":
                    dict2["b"] = i
                if i[0] == "H":
                    dict2["c"] = i
                if i[0] == "S":
                    dict2["d"] = i
                x2 = list(dict2.popitem())
                ls6.append(x2)
            ls6.sort(reverse = True ,key = lambda x:x[0])
            for i in ls6:
                ans.append(i[1])
            ls6.clear()
            ls5.clear()
    hd_cards.clear()
    for i in ans:
        hd_cards.append(i)
    for i in hd_cards:
        if i == "":
            hd_cards.pop(i)
    while rd_num > 0:
        hd_num = len(hd_cards)
        rd_num -= 1
        ans = []
        ans.clear()
        atk_cards = input().split(",")
        for i in atk_cards:
            if i[0] == "A":
                atk_cards.remove(i)
                atk_cards.insert(0,i)
            if i[0] == "D":
                atk_cards.insert(len(atk_cards)-1,i)
                atk_cards.remove(i)
        atk_num = len(atk_cards)
        x , y , m , n= -1 , -1 , 0 , -1
        num = ["A","K","Q","J","T","9","8","7","6","5","4","3","2"]
        color = ["S","H","C","D"]
        lt1 = []
        hd_cards1 = hd_cards.copy()
        if atk_num == 1:
            for i in range(len(num)):
                x += 1
                if num[x] == atk_cards[0][1]:
                    for j in range(len(color)):
                        y += 1
                        if color[y] == atk_cards[0][0]:                                
                            for k in range(len(hd_cards)):
                                m -= 1
                                if hd_cards[m][1] in num[0:x+1] and hd_cards[m][0] in color[0:y+1]:
                                    lt1.append(hd_cards[m])
                                    m += 1
            if lt1 == []:
                final_ans.append("Pass")
                hp -= 1
            if lt1 != []:
                final_ans.append(lt1[0])
                hd_cards.remove(lt1[0])
                lt1.clear()
        if atk_num == 2:
            for i in range(len(num)):
                x += 1
                if num[x] == atk_cards[0][1]:
                    for j in range(len(color)):
                        y += 1
                        if color[y] == atk_cards[0][0]:                                
                            for k in range(len(hd_cards)):
                                m -= 1
                                if hd_cards[m][1] in num[0:x+1] and hd_cards[m][0] in color[0:y+1]:
                                    lt1.append(hd_cards1.pop(m))
                                    m += 1
            if len(lt1) <= 1:
                final_ans.append("Pass")
                hp -= 1
            if len(lt1) >= 2:
                for i in range(len(lt1)-1):
                    n += 1
                    if lt1[n][1] == lt1[n+1][1]:
                        ans.append(lt1[n+1])
                        hd_cards.remove(lt1[n+1])
                        ans.append(lt1[n])
                        hd_cards.remove(lt1[n])
                        final_ans.append(ans)
                        lt1.clear()
                        break
                    if lt1[n][1] != lt1[n+1][1] and n == len(lt1) - 2:
                        final_ans.append("Pass")
                        hp -= 1
                        break
        if atk_num == 3:
            for i in range(len(num)):
                x += 1
                if num[x] == atk_cards[0][1]:
                    for j in range(len(color)):
                        y += 1
                        if color[y] == atk_cards[0][0]:                                
                            for k in range(len(hd_cards)):
                                m -= 1
                                if hd_cards[m][1] in num[0:x]:
                                    lt1.append(hd_cards1.pop(m))
                                    m += 1
            if len(lt1) <= 2:
                final_ans.append("Pass")
                hp -= 1
            if len(lt1) >= 3:
                for i in range(len(lt1)-2):
                    n += 1
                    if lt1[n][1] == lt1[n+2][1]:
                        ans.append(lt1[n+2])
                        hd_cards.remove(lt1[n+2])
                        ans.append(lt1[n+1])
                        hd_cards.remove(lt1[n+1])
                        ans.append(lt1[n])
                        hd_cards.remove(lt1[n])
                        final_ans.append(ans)
                        lt1.clear()
                        break
                    if lt1[n][1] != lt1[n+2][1] and n == len(lt1) - 3:
                        final_ans.append("Pass")
                        hp -= 1
                        break
        if atk_num == 4:
            for i in range(len(num)):
                x += 1
                if num[x] == atk_cards[0][1]:
                    for j in range(len(color)):
                        y += 1
                        if color[y] == atk_cards[0][0]:                                
                            for k in range(len(hd_cards)):
                                m -= 1
                                if hd_cards[m][1] in num[0:x]:
                                    lt1.append(hd_cards1.pop(m))
                                    m += 1
            if len(lt1) <= 3:
                final_ans.append("Pass")
                hp -= 1
            if len(lt1) >= 4:
                for i in range(len(lt1)-3):
                    n += 1
                    if lt1[n][1] == lt1[n+3][1]:
                        ans.append(lt1[n+3])
                        hd_cards.remove(lt1[n+3])
                        ans.append(lt1[n+2])
                        hd_cards.remove(lt1[n+2])
                        ans.append(lt1[n+1])
                        hd_cards.remove(lt1[n+1])
                        ans.append(lt1[n])
                        hd_cards.remove(lt1[n])
                        final_ans.append(ans)
                        lt1.clear()
                        break
                    if lt1[n][1] != lt1[n+3][1] and n == len(lt1) - 4:
                        final_ans.append("Pass")
                        hp -= 1
                        break
        if hp == 0:
            for i in final_ans:
                print(i)
            print("Twisted Fate lost all his HP and lost.")
        if hp > 0 and rd_num == 0 and hd_num > 0:
            for i in final_ans:
                print(i)
            print("Twisted Fate won with {} HP left.".format(hp))
        if hp > 0 and rd_num > 0 and hd_num == 0:
            for i in final_ans:
                print(i)
            print("Twisted Fate won with {} HP left.".format(hp))
        if hp > 0 and rd_num == 0 and hd_num == 0:
            for i in final_ans:
                print(i)
            print("Twisted Fate won with {} HP left.".format(hp))
        if hd_num == 0 or hp == 0:
            rd_num = 0                    
#====================================YOUR CODE HERE================================================

def main(): # DO NOTE DELETE THIS FUNCTION!
    #==============================================================================
    # Note: if you want oj evaluate your code, uncomment the corresponding function.
    #==============================================================================
    
    # Act1("stacked.in")
    Act2()
    # Act3()
    # Act4()
    return 0

if __name__ == '__main__':
    main()                 
                                
                                






















                
