# ===============================---==NOTE: do not use Hardcode==---===============================
# ===============================---==NOTE: do not use Libraries=---===============================


# ========================================TASK1====================================================
def Act1(PATH: str):
    inp = open(PATH , "r")
    ls1 = []#create a empty list
    st = set(ls1)
    for line in inp:
        ls1.append(line.strip('\n').split(","))
    for row in ls1:
        for item in row:
            item1 = item.replace(" ","")
            if item1 != "" and item1 != " ":
                st.add(item1)
    ls2 = list(st)#transform the set to a new list to complete the first sort(the first comparation)
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
    print(ans)


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
        hd_cards.append(i.replace("D","B").replace("A","Z").replace("K","R").replace("T","I"))
    for i in hd_cards:
        if i == "":
            hd_cards.pop(i)
    hd_num = len(hd_cards)
    ans = []
    while rd_num > 0:
        rd_num -= 1
        ans.clear()
        atk_cards = []
        atk_cards10 = input().replace("D","B").replace("A","Z").replace("K","R").replace("T","I").replace(" ","").split(",")
        for i in atk_cards10:
            if i != "" and i!= " ":
                atk_cards.append(i)
        atk_num = len(atk_cards)
        x , y , m , n= 0 , 0 , 0 , 0
        hd_num1 = hd_num#assumed number
        hd_cards1 = hd_cards#assumed cards
        if atk_num == 1:
            while x > - hd_num:
                x -= 1
                if hd_cards[0][1] < atk_cards[0][1]:
                    final_ans.append("Pass")
                    hp -= 1
                    x = - hd_num
                if hd_cards[0][1] == atk_cards[0][1]:
                    if hd_cards[0][0] <= atk_cards[0]:
                        final_ans.append("Pass")
                        hp -= 1
                        x = - hd_num
                if hd_cards[x][1] == atk_cards[0][1] and hd_cards[x][0] > atk_cards[0][0]:
                    re = hd_cards.pop(x).replace("B","D").replace("Z","A").replace("R","K").replace("I","T")
                    ans.append(re)
                    hd_num -= 1
                    x = -hd_num
                    final_ans.append(ans)
                if hd_cards[x][1] > atk_cards[0][1]:
                    re = hd_cards.pop(x).replace("B","D").replace("Z","A").replace("R","K").replace("I","T")
                    ans.append(re)
                    hd_num -= 1
                    x = - hd_num
                    final_ans.append(ans)
        if atk_num == 2:
            while x > - hd_num:
                x -= 1
                if hd_cards[1][1] < atk_cards[0][1]:
                    final_ans.append("Pass")
                    hp -= 1
                    x = - hd_num
                elif hd_cards[1][1] == atk_cards[0][1]:
                    if hd_cards[1][0] <= atk_cards[0][0]:
                        final_ans.append("Pass")
                        hp -= 1
                        x = - hd_num
                elif hd_cards[x][1] == atk_cards[0][1] and hd_cards[x][0] > atk_cards[0][0] and x - 1>= -hd_num:
                    if hd_cards[x][1] == hd_cards[x-1][1]:
                        re = hd_cards.pop(x).replace("B","D").replace("Z","A").replace("R","K").replace("I","T")
                        ans.append(re)
                        hd_num1 -= 1
                        re = hd_cards.pop(x).replace("B","D").replace("Z","A").replace("R","K").replace("I","T")
                        ans.append(re)
                        hd_num -= 1
                        x = -hd_num-4
                elif hd_cards[x][1] > atk_cards[0][1] and x - 1>= -hd_num:
                    if hd_cards[x][1] == hd_cards[x-1][1]:
                        re = hd_cards.pop(x).replace("B","D").replace("Z","A").replace("R","K").replace("I","T")
                        ans.append(hd_cards.pop(x))
                        hd_num -= 1
                        re = hd_cards.pop(x).replace("B","D").replace("Z","A").replace("R","K").replace("I","T")
                        ans.append(re)
                        hd_num -= 1
                        x = -hd_num-4
        if atk_num == 3:
            while x > -hd_num1:
                x -= 1
                if hd_cards1[0][1] < atk_cards[2][1]:
                    final_ans.append("Pass")
                    hp -= 1
                    x = -hd_num1
                if hd_cards1[x][1] > atk_cards[2][1] and x-2 < -hd_num1:
                    final_ans.append("Pass")
                    hp -= 1
                    x = -hd_num1
                if hd_cards1[x][1] == atk_cards[2][1] and x - 2 <= -hd_num1:
                    final_ans.append("Pass")
                    hp -= 1
                    x = -hd_num1
                if hd_cards1[x][1] > atk_cards[2][1] and x - 2 >= -hd_num1:
                    if hd_cards[x-2][1] == hd_cards[x][1]:
                        re = hd_cards1.pop(y).replace("B","D").replace("Z","A").replace("R","K").replace("I","T")
                        hd_cards1.insert(y,re)
                        re = hd_cards1.pop(y-1).replace("B","D").replace("Z","A").replace("R","K").replace("I","T")
                        hd_cards1.insert(y-1,re)
                        re = hd_cards1.pop(y-2).replace("B","D").replace("Z","A").replace("R","K").replace("I","T")
                        hd_cards1.insert(y-2,re)
                        ans.append(hd_cards[x-2])
                        ans.append(hd_cards[x-1])
                        ans.append(hd_cards[x])
                        final_ans.append(ans)
                        x = -hd_num1
        if atk_num == 4:
            while x > -hd_num1:
                x -= 1
                if hd_cards1[0][1] < atk_cards[3][1]:
                    final_ans.append("Pass")
                    hp -= 1
                    x = -hd_num1
                if hd_cards1[x][1] > atk_cards[3][1] and x-3 < -hd_num1:
                    final_ans.append("Pass")
                    hp -= 1
                    x = -hd_num1
                if hd_cards1[x][1] == atk_cards[3][1] and x-3 <= -hd_num1:
                    final_ans.append("Pass")
                    hp -= 1
                    x = -hd_num1
                if hd_cards1[x][1] > atk_cards[3][1] and x-3 >= -hd_num1:
                    if hd_cards[x-3][1] == hd_cards[x][1]:
                        re = hd_cards1.pop(y).replace("B","D").replace("Z","A").replace("R","K").replace("I","T")
                        hd_cards1.insert(y,re)
                        re = hd_cards1.pop(y-1).replace("B","D").replace("Z","A").replace("R","K").replace("I","T")
                        hd_cards1.insert(y-1,re)
                        re = hd_cards1.pop(y-2).replace("B","D").replace("Z","A").replace("R","K").replace("I","T")
                        hd_cards1.insert(y-2,re)
                        re = hd_cards1.pop(y-3).replace("B","D").replace("Z","A").replace("R","K").replace("I","T")
                        hd_cards1.insert(y-3,re)
                        ans.append(hd_cards[x-3])
                        ans.append(hd_cards[x-2])
                        ans.append(hd_cards[x-1])
                        ans.append(hd_cards[x])
                        final_ans.append(ans)
                        x = -hd_num1
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



# ========================================TASK3====================================================
def Act3():
#====================================YOUR CODE HERE================================================
    pass
#====================================YOUR CODE HERE================================================


        
# ===================================TASK4=========================================================
def Act4():
#====================================YOUR CODE HERE================================================
    cards_temp = input().replace(" ","").split(",")
    cards = []
    S = []
    H = []
    C = []
    D = []
    for i in cards_temp:
        if i != "" and i != " ":
            cards.append(i)
    ls2 = cards
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
    cards.clear()
    for i in ans:
        cards.append(i)
    for i in cards:
        if i[0] == "S":
            S.append(i)
        if i[0] == "H":
            H.append(i)
        if i[0] == "C":
            C.append(i)
        if i[0] == "D":
            D.append(i)
    x,y,m,n = -1,-1,-1,-1
    a,b,c,d = [],[],[],[]
    if len(S) > 4:
        for i in range(len(S)-4):
            x += 1
            BoardS = S[x][1]+ S[x+1][1]+ S[x+2][1]+ S[x+3][1]+ S[x+4][1]
            if BoardS in ["AKQJT","KQJT9","QJT98","JT987","T9876","98765","87654","76543","65432"]:
                a.append("True")
            else:
                a.append("False")
    if len(H) > 4:
        for i in range(len(H)-4):
            y += 1
            BoardH = H[y][1]+ H[y+1][1]+ H[y+2][1]+ H[y+3][1]+ H[y+4][1]
            if BoardH in ["AKQJT","KQJT9","QJT98","JT987","T9876","98765","87654","76543","65432"]:
                b.append("True")
            else:
                b.append("False")

    if len(C) > 4:
        for i in range(len(C)-4):
            m += 1
            BoardC = C[m][1]+ C[m+1][1]+ C[m+2][1]+ C[m+3][1]+ C[m+4][1]
            if BoardC in ["AKQJT","KQJT9","QJT98","JT987","T9876","98765","87654","76543","65432"]:
                c.append("True")
            else:
                c.append("False")
    if len(D) > 4:
        for i in range(len(D)-4):
            n += 1
            BoardD = D[n][1]+ D[n+1][1]+ D[n+2][1]+ D[n+3][1]+ D[n+4][1]
            if BoardD in ["AKQJT","KQJT9","QJT98","JT987","T9876","98765","87654","76543","65432"]:
                d.append("True")
            else:
                d.append("False")
    if len(S) <= 4:
        a.append("False")
    if len(H) <= 4:
        b.append("False")
    if len(C) <= 4:
        c.append("False")
    if len(D) <= 4:
        d.append("False")
    if "True" in a or "True" in b or "True" in c or "True" in d:
        print("True")
    if "False" in a and "False" in b and "False" in c and "False" in d and "True" not in a and "True" not in b and "True" not in c and "True" not in d:
        print("False")
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
