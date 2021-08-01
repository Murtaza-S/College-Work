import sys
current_user = -1
count = 0
user_count = 0


for line in sys.stdin:
    users = line.split(" ")
    if int(users[0]) == current_user:
        count = count +  1
    else:
        if count >= 20 :
            #print(current_user)
            user_count = user_count + 1
        count = 0
        current_user = int(users[0])

print("Budget for ad campaign : INR ", 100*user_count)

