import sys
curr_index = (-1, -1)
curr_mat = "XX"
count = 0
check = -1

for line in sys.stdin : 
    line = line.strip("\n")
    #print(line)
    values = line.split(" ")
    #print(values)

    index = (values[0],values[1])
    
    if index != curr_index :
        i1, i2 = int(curr_index[0]), int(curr_index[1]) 
        if curr_index[0] != -1 and curr_index[0] != curr_index[1]:
            print(f"{i1:04} {i2:04} {count}")

        curr_index = index
        curr_mat = values[3]
        check = values[2]
        count = 0
        

    else :
        if check == values[2] and curr_mat != values[3]:
            count += 1
        else : 
            curr_mat = values[3]
            check = values[2]
