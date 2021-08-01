file = open("network.txt")
user_map = []


for line in file:
    users = line.split(",")
    users[1] = users[1].rstrip("\n")
    users[0] = int(users[0])
    users[1] = int(users[1])
    
    if users[0] < users[1]:
        print(f"{users[0]:04} {users[1]:04}")
    else:
        print(f"{users[1]:04} {users[0]:04}")
    
    #print(f"{users[0]:04}", " ", 1)
    #print(f"{users[1]:04}", " ", 1)
    
    
    
    #user_map.append([users[0],1])
    #user_map.append([users[1],1])

