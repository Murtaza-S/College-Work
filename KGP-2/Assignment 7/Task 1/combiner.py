import sys
curr = -1
count = 0
for line in sys.stdin : 
    values = line.split(" ")
    
    if curr == values[0] : 
        count = count + 1
    else :
        if curr != -1 :
            #print(curr, " ", count)
            users = curr.split(":")
            #print(users)
            users[0] = int(users[0])
            users[1] = int(users[1])
            #print(users)
            print(f"{users[0]:04}:{users[1]:04} {count}")

        curr = values[0]
        count = int(values[1])
        
print(f"{users[0]:04}:{users[1]:04} {count}")