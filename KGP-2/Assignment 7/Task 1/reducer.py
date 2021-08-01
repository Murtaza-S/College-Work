import sys
curr = -1
count = 0
usercount = 0
for line in sys.stdin : 
    line = line.strip("\n")
    #print(line)
    values = line.split(" ")
    #print(values)
    
    if curr == values[0] : 
        count = count + int(values[1])
    else :
        if curr != -1 and count >= 30 :
            usercount += 1
            print(curr, " ", usercount)

        curr = values[0]
        count = int(values[1])
        
        #else :
            #if count >= 30 :
            #    usercount += 1
            
            #curr = values[0]
            #count = int(values[1])
        
#print(curr, " ", count)
if count >= 30 :
    usercount += 1
    print(curr, " ", usercount)

print("User strongly attached are : ", usercount)