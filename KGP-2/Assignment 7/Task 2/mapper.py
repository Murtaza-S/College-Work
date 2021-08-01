import sys

for line in sys.stdin : 

    users = line.split(",")
    users[1] = users[1].rstrip("\n")

    users[0] = int(users[0])
    users[1] = int(users[1])

    k = 0

    while k < 300:
        print(f"{users[0]:04} {k:04} {users[1]:04} A1")
        print(f"{users[1]:04} {k:04} {users[0]:04} A1")
        
        print(f"{k:04} {users[0]:04} {users[1]:04} A2")
        print(f"{k:04} {users[1]:04} {users[0]:04} A2")
        k += 1
    
    