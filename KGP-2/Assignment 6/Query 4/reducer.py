import sys

def list_min_user(top_us_users, us_users):
    
    min_user, min_count = top_us_users [0], us_users [top_us_users[0]] [1]
    
    for item in top_us_users:
        
        if us_users [item] [1] < min_count:

            min_count = us_users [item] [1]
            min_user = item

    #print(min_user, " ", min_count)
    return min_user, min_count



#Function to give top US users w.r.t US friends
def give_top_us_users(us_users):
    
    top_us_users = []

    user_count = 0
    for item in range(1000):
        user_count = user_count + 1

        #Appends in list for first 10 users
        if user_count < 10 :
            top_us_users.append(item)
            min_user, min_count = list_min_user(top_us_users, us_users)

        #Checks if the minimum US friends count in current list is suppressed by current user
        #If so it pops off the User with min US friends and add the Current User
        else:
            
            if us_users [item] [1] > min_count : 
                top_us_users.remove(min_user)
                top_us_users.append(item)
                min_user, min_count = list_min_user(top_us_users, us_users)
        
    return top_us_users

#List to track all US users friends
us_users = []

for _ in range(1000):
    us_users.append([0,0])
    #the first count denotes total friends and second count denotes friends in US


for line in sys.stdin:
    users = line.split(" ")
    users[0] = int(users[0])
    users[1] = int(users[1])

    if 0 <= users[0] <= 999 and 0 <= users[1] <= 999:

        #Adding count to both as being a friend and a US friend for both users
        us_users [users[0]] [0] =  us_users [users[0]] [0] + 1
        us_users [users[0]] [1] =  us_users [users[0]] [1] + 1

        us_users [users[1]] [0] =  us_users [users[1]] [0] + 1
        us_users [users[1]] [1] =  us_users [users[1]] [1] + 1

    elif 0 <= users[0] <= 999:

        #Adding count as only a friend to US user
        us_users [users[0]] [0] =  us_users [users[0]] [0] + 1

    elif 0 <= users[1] <= 999:

        #Adding count as only a friend to US user
        us_users [users[1]] [0] =  us_users [users[1]] [0] + 1
    
top_us_users = give_top_us_users(us_users)

#print(top_us_users)

budget = 0
for _ in top_us_users:
    budget = budget + us_users [_] [0]

print("Budget to spend on top US users : INR ", 10*budget)