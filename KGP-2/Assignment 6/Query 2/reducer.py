import sys

#Function to find minimum key : value pair
def find_min_key(top_users):
    user_items = list(top_user.items())
    m = user_items[0][1]
    min_key = user_items[0][0]
    for k,v in user_items:
        if v < m:
            #print("alpha : ", k, " ", v)
            min_key = k
            m = v

    return min_key, m


current_user = -1
count = 0
user_count = 0
top_user = {}


for line in sys.stdin:
    users = line.split(" ")
    
    
    if int(users[0]) == current_user:
        count = count +  1
        
    else:
        if user_count > 0 :
            #Makes a dict of first 10 users
            if user_count <= 10 :
                #print(current_user)
                top_user[current_user] = count
            
            #Changes the top users dictionary when nessesory
            else :
                min_key, min_val = find_min_key(top_user)
                if count > min_val:
                    #print("Key : ", min_key, " Value : ", min_val)
                    top_user.pop(min_key)
                    top_user[current_user] = count
                    #print(top_user)

        user_count = user_count + 1
        count = 1
        current_user = int(users[0])

#Print the list of top 10 connected users in the dataset 
for _ in top_user.keys():
    print(_)

